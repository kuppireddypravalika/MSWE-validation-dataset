#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <string>
#include <cmath>

struct Matrix3 {
    double m[3][3];
};

struct Image {
    int w, h;
    std::vector<double> data;
    Image(int w=0, int h=0): w(w), h(h), data(w*h) {}
    double& operator()(int x, int y) { return data[y*w + x]; }
    const double& operator()(int x, int y) const { return data[y*w + x]; }
};

void resample(const Matrix3& H0, const Matrix3& H1,
              const Image& imgL, const Image& imgR,
              Image& rectL, Image& rectR);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 10000;
    int size = 256;

    for(int i=1;i<argc;++i) {
        std::string arg = argv[i];
        if(arg=="--mode=correct") mode="correct";
        else if(arg=="--mode=perf") mode="perf";
        else if(arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if(arg.rfind("--size=",0)==0) size = std::stoi(arg.substr(7));
    }

    Image imgL(size,size), imgR(size,size);
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0,1.0);
    for(int i=0;i<size;++i)
        for(int j=0;j<size;++j) {
            imgL(j,i) = dist(rng);
            imgR(j,i) = dist(rng);
        }

    Matrix3 H0{{{1,0,0},{0,1,0},{0.001,0.002,1}}};
    Matrix3 H1{{{0.999,0.01,0},{-0.01,1,0},{-0.002,0.001,1}}};
    Image rectL, rectR;

    if(mode=="correct") {
        resample(H0,H1,imgL,imgR,rectL,rectR);
        double sum=0.0;
        for(double v: rectL.data) sum+=v;
        for(double v: rectR.data) sum+=v;
        std::cout<<sum<<std::endl;
    } else {
        using clock=std::chrono::high_resolution_clock;
        auto t1=clock::now();
        for(int i=0;i<repeat;++i) {
            resample(H0,H1,imgL,imgR,rectL,rectR);
        }
        auto t2=clock::now();
        std::cout<<"Time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()<<" ms\n";
    }
    return 0;
}


