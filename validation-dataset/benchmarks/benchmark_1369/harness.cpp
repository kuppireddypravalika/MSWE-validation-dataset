#include "image.hpp"
#include <chrono>
#include <random>
#include <iostream>
#include <string>

Image make_image(int w, int h)
{
    Image img(w, h);
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(0, 255);
    for (int i = 0; i < w*h; ++i) img.data[i] = dist(gen);
    return img;
}

void downsample_image(const Image& src, Image& dst);

int main(int argc, char* argv[])
{
    std::string mode = "perf";
    int size = 2048; // square image
    int repeat = 50000;
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=",0)==0) size = std::stoi(arg.substr(7));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    Image src = make_image(size, size);
    Image dst;

    if (mode == "correct")
    {
        downsample_image(src, dst);
        long long sum = 0;
        for (int i = 0; i < dst.sx*dst.sy; ++i) sum += dst.data[i];
        std::cout << sum << '\n';
    }
    else
    {
        using clock = std::chrono::high_resolution_clock;
        long long dummy = 0;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
        {
            downsample_image(src, dst);
            dummy += dst.data[0];
        }
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
        if (dummy == 123456) std::cerr << "";
    }
    return 0;
}
