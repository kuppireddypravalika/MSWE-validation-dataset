#include <chrono>
#include <iostream>
#include <string>
#include <vector>

float build(const std::vector<float>& sah, float leafSAH, const std::vector<float>& data);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 1;
    size_t nvals = 2000;
    size_t datasize = 1024;
    float leafSAH = 100.0f;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--nvals=",0)==0) nvals = std::stoul(arg.substr(8));
        else if (arg.rfind("--datasize=",0)==0) datasize = std::stoul(arg.substr(11));
    }

    std::vector<float> sah(nvals);
    for (size_t i = 0; i < nvals; ++i)
        sah[i] = leafSAH * (0.55f + 0.002f * (i % 100));

    std::vector<float> data(datasize);
    for (size_t i = 0; i < datasize; ++i)
        data[i] = static_cast<float>(i) * 0.001f;

    if (mode == "correct") {
        float res = 0.0f;
        res = build(sah, leafSAH, data);
        std::cout << res << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            build(sah, leafSAH, data);
        auto t2 = clock::now();

        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }
}
