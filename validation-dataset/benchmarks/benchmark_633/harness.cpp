#include "conv_common.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <random>

using namespace bench633;
namespace bench633 {
void do_conv(const AddMatrixExpr& a, const AddMatrixExpr& b, Matrix& c);
}
Matrix make_matrix(std::size_t r, std::size_t c) {
    Matrix m(r,c);
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(0.0,1.0);
    for(std::size_t i=0;i<r;++i)
        for(std::size_t j=0;j<c;++j)
            m(i,j)=dist(gen);
    return m;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t n = 128;
    std::size_t k = 7;
    int repeat = 16000;


    for (int i=1; i<argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode="correct";
        else if (arg == "--mode=perf") mode="perf";
        else if (arg.rfind("--size=",0)==0) n = std::stoul(arg.substr(7));
        else if (arg.rfind("--kernel=",0)==0) k = std::stoul(arg.substr(9));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    Matrix a1 = make_matrix(n,n);
    Matrix a2 = make_matrix(n,n);
    Matrix k1 = make_matrix(k,k);
    Matrix k2 = make_matrix(k,k);

    AddMatrixExpr input(a1,a2);
    AddMatrixExpr kernel(k1,k2);
    Matrix out(n-k+1, n-k+1);

    if(mode=="correct") {
        do_conv(input, kernel, out);
        double sum=0.0;
        for(std::size_t i=0;i<out.rows();++i)
            for(std::size_t j=0;j<out.cols();++j)
                sum += out(i,j);
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for(int r=0;r<repeat;++r) {
            do_conv(input, kernel, out);
        }
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << " ms\n";
        if(out(0,0)==-1) std::cerr << "";
    }
    return 0;
}
