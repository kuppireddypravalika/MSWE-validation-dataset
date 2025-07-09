#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <algorithm>

struct Matrix {
    size_t rows_, cols_;
    std::vector<double> data;
    Matrix(size_t r=0, size_t c=0) : rows_(r), cols_(c), data(r*c) {}
    double& operator()(size_t i, size_t j) { return data[i*cols_ + j]; }
    const double& operator()(size_t i, size_t j) const { return data[i*cols_ + j]; }
    size_t rows() const { return rows_; }
    size_t columns() const { return cols_; }
};

Matrix make_upper_matrix(size_t n) {
    Matrix m(n,n);
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(0.0,1.0);
    for(size_t i=0;i<n;++i){
        for(size_t j=0;j<n;++j){
            if(j>=i) m(i,j)=dist(gen); else m(i,j)=0.0;
        }
    }
    return m;
}

void smat_smat_mult(const Matrix& lhs, const Matrix& rhs, Matrix& res); // provided by impl

int main(int argc, char** argv){
    std::string mode = "perf";
    size_t size = 200;
    int repeat = 7000; // baseline, adjust later

    for(int i=1;i<argc;++i){
        std::string arg = argv[i];
        if(arg == "--mode=correct") mode = "correct";
        else if(arg == "--mode=perf") mode = "perf";
        else if(arg.rfind("--size=",0)==0) size = std::stoul(arg.substr(7));
        else if(arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    Matrix A = make_upper_matrix(size);
    Matrix B = make_upper_matrix(size);
    Matrix C(size,size);

    if(mode=="correct"){
        smat_smat_mult(A,B,C);
        double sum=0.0;
        for(double v : C.data) sum += v;
        std::cout << sum << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    double dummy = 0.0;
    auto t1 = clock::now();
    for(int r=0;r<repeat;++r){
        smat_smat_mult(A,B,C);
        dummy += C(0,0);
    }
    auto t2 = clock::now();
    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    if(dummy==12345.678) std::cerr << dummy;
    return 0;
}
