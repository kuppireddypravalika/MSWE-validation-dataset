#include "song_filter.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>


static std::vector<const char*> make_args(std::size_t pairs) {
    std::vector<const char*> v;
    v.reserve(pairs * 2);
    for (std::size_t i = 0; i < pairs; ++i) {
        v.push_back("tag");
        v.push_back("value");
    }
    return v;
}

int main(int argc, char** argv) {
    std::string mode = "perf";
    std::size_t pairs = 50;
    // repeat count tuned so the original implementation runs \u2265 10s
    int repeat = 15000000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--pairs=",0)==0) pairs = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    std::vector<const char*> arg_data = make_args(pairs);
    ConstBuffer<const char*> buffer{arg_data.data(), arg_data.size()};

    if (mode == "correct") {
        SongFilter f;
        f.items.reserve(pairs);
        f.Parse(buffer, true);
        long long sum = 0;
        for (const auto& it : f.items) sum += it.tag.size() + it.value.size();
        std::cout << sum << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    long long dummy = 0;
    auto t1 = clock::now();
    for (int r = 0; r < repeat; ++r) {
        SongFilter f;
        f.items.reserve(pairs);
        ConstBuffer<const char*> b{arg_data.data(), arg_data.size()};
        f.Parse(b, true);
        dummy += f.items.size();
    }
    auto t2 = clock::now();
    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    if (dummy == 123456789) std::cerr << dummy;
    return 0;
}
