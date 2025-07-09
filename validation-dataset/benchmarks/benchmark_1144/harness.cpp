#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <string>

// function from benchmark sources
void process_views(std::vector<std::vector<double>>& views);

static std::vector<std::vector<double>> create_views(int nViews, int viewSize) {
    std::vector<std::vector<double>> views(nViews, std::vector<double>(viewSize));
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for(auto& v : views)
        for(double& x : v) x = dist(gen);
    return views;
}

int main(int argc, char** argv) {
    std::string mode = "perf";
    int nViews = 400;
    int viewSize = 1000;
    int repeat = 1200;

    for(int i=1;i<argc;++i){
        std::string arg = argv[i];
        if(arg == "--mode=correct") mode = "correct";
        else if(arg == "--mode=perf") mode = "perf";
        else if(arg.rfind("--views=",0)==0) nViews = std::stoi(arg.substr(8));
        else if(arg.rfind("--size=",0)==0) viewSize = std::stoi(arg.substr(7));
        else if(arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    auto base = create_views(nViews, viewSize);

    if(mode == "correct") {
        auto data = base;
        process_views(data);
        double sum = 0.0;
        for(const auto& v : data)
            for(double x : v) sum += x;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for(int r=0;r<repeat;++r){
            auto data = base;
            process_views(data);
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
                  << " ms\n";
    }
}
