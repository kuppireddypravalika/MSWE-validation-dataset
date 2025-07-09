#include <chrono>
#include <iostream>
#include <string>
#include <vector>

size_t handler(const std::string& input);

std::string make_message(size_t size) {
    std::string s;
    s.reserve(size);
    for (size_t i = 0; i < size; ++i)
        s.push_back('a' + (i % 26));
    return s;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t repeat = 45000000;
    size_t msg_size = 1024;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoul(arg.substr(9));
        else if (arg.rfind("--size=", 0) == 0) msg_size = std::stoul(arg.substr(7));
    }

    std::string msg = make_message(msg_size);
    size_t sink = 0;

    if (mode == "correct") {
        sink = handler(msg);
        std::cout << sink << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (size_t i = 0; i < repeat; ++i)
            sink += handler(msg);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    // Prevent dead-code elimination
    if (sink == 42) std::cerr << "";
    return 0;
}
