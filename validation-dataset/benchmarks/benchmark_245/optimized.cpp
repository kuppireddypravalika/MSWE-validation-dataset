#include <vector>
#include <cstddef>
#include <algorithm>

struct Matrix {
    size_t rows_, cols_;
    std::vector<double> data;
    Matrix(size_t r=0,size_t c=0):rows_(r),cols_(c),data(r*c){}
    double& operator()(size_t i,size_t j){ return data[i*cols_+j]; }
    const double& operator()(size_t i,size_t j) const{ return data[i*cols_+j]; }
    size_t rows() const { return rows_; }
    size_t columns() const { return cols_; }
};

void smat_smat_mult(const Matrix& lhs, const Matrix& rhs, Matrix& res){
    const size_t N = lhs.rows();
    for(size_t i=0;i<N;++i){
        for(size_t j=0;j<N;++j){
            double tmp = 0.0;
            if(lhs.columns() != 0UL){
                const size_t kbegin( std::max(i, size_t(0)) );
                const size_t kend  ( std::min(lhs.columns(), j+1) );
                tmp = lhs(i,kbegin) * rhs(kbegin,j);
                for(size_t k=kbegin+1UL; k<kend; ++k){
                    tmp += lhs(i,k) * rhs(k,j);
                }
            }
            res(i,j) = tmp;
        }
    }
}
