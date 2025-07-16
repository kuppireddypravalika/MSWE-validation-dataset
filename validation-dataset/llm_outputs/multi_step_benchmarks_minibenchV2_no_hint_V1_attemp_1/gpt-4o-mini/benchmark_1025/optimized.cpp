// original.cpp
#include <vector>
#include <cstring>
#include <omp.h>

#define SPLA_OMP_PRAGMA(x) _Pragma(x)

template <typename T>
struct Matrix {
    int cols, rows;
    std::vector<T> data;
    Matrix(int c, int r) : cols(c), rows(r), data(c * r) {}
    T* ptr(int c, int r) { return data.data() + c * rows + r; }
    T& operator()(int c, int r) { return data[c * rows + r]; }
    const T& operator()(int c, int r) const { return data[c * rows + r]; }
};

struct BlockInfo {
    int globalSubRowIdx;
    int globalSubColIdx;
    int numRows;
    int numCols;
    int localRowIdx;
    int localColIdx;
    int mpiRank;
};

template <typename T>
struct TileHost {
    Matrix<T> C;
    Matrix<T> tile;
    std::vector<BlockInfo> blockInfos;
    int rank;
    int numThreads;
    T beta;
    TileHost(int rows, int cols, int threads)
        : C(cols, rows), tile(cols, rows), rank(0), numThreads(threads), beta(0) {
        blockInfos.push_back({0, 0, rows, cols, 0, 0, 0});
        for (int c = 0; c < cols; ++c)
            for (int r = 0; r < rows; ++r)
                tile(c, r) = static_cast<T>(r + c * rows);
    }
    void extract() {
        for (const auto& info : blockInfos) {
            int tileRowOffset = info.globalSubRowIdx - blockInfos.front().globalSubRowIdx;
            int tileColOffset = info.globalSubColIdx - blockInfos.front().globalSubColIdx;
            if (info.mpiRank == rank || info.mpiRank < 0) {
                T* C_ptr_base = C.ptr(info.localColIdx, info.localRowIdx);
                const T* tile_ptr_base = tile.ptr(tileColOffset, tileRowOffset);
                if (beta == T(0.0) || beta == T(-0.0)) {
                    SPLA_OMP_PRAGMA("omp parallel for schedule(static) num_threads(numThreads)")
                    for (int col = 0; col < info.numCols; ++col) {
                        std::memcpy(C_ptr_base + col * info.numRows,
                                    tile_ptr_base + col * info.numRows,
                                    info.numRows * sizeof(T));
                    }
                } else {
                    SPLA_OMP_PRAGMA("omp parallel for schedule(static) num_threads(numThreads)")
                    for (int col = 0; col < info.numCols; ++col) {
                        T* C_ptr = C_ptr_base + col * info.numRows;
                        const T* tile_ptr = tile_ptr_base + col * info.numRows;
                        for (int row = 0; row < info.numRows; ++row) {
                            C_ptr[row] = beta * C_ptr[row] + tile_ptr[row];
                        }
                    }
                }
            }
        }
    }
};

extern "C" double run(size_t repeat) {
    const int size = 32;
    TileHost<double> host(size, size, omp_get_max_threads());
    double sum = 0.0;
    for (size_t i = 0; i < repeat; ++i) {
        host.extract();
        sum += host.C(0, 0);
    }
    return sum;
}

// Explicit template instantiations
template struct Matrix<double>;
template struct TileHost<double>;