#include "sequence.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <random>

// function declarations

template <typename Seq>
void sort_sequence(Seq& seq);

template <typename T>
Sequence<T> make_sequence(std::size_t n) {
    Sequence<T> seq(n);
    std::mt19937 gen(42);
    std::uniform_int_distribution<T> dist(0, 1000000);
    for (std::size_t i = 0; i < n; ++i) seq.data[i] = dist(gen);
    return seq;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    
    std::size_t size = 10000;
    int repeat = 40000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--size=",0)==0)
            size = static_cast<std::size_t>(std::stoul(arg.substr(7)));
        else if (arg.rfind("--repeat=",0)==0)
            repeat = std::stoi(arg.substr(9));
    }

    Sequence<int> base = make_sequence<int>(size);

    if (mode == "correct") {
        Sequence<int> seq = base;
        sort_sequence(seq);
        long long sum = 0;
        for (int v : seq.data) sum += v;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        long long dummy = 0;

        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            Sequence<int> seq = base;
            sort_sequence(seq);
            dummy += seq.data[0];
        }
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 123456789) std::cerr << "";
    }
    return 0;
}
