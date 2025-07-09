#include <chrono>
#include <iostream>
#include <string>
#include <vector>

// Implementation is provided by either original.cpp or optimized.cpp
// depending on which binary is being built.
const char *check_secure_area(const std::vector<uint8_t> &rom);

int main(int argc, char **argv)
{
    std::string mode = "perf";      // perf or correct
    std::string impl = "original";  // parsed for compatibility (unused)
    long long repeat = 15000000;    // number of iterations for perf mode

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg == "--impl=optimized")
            impl = "optimized";
        else if (arg == "--impl=original")
            impl = "original";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoll(arg.substr(9));
    }

    // Construct a dummy ROM image. Most bytes are zero except for a
    // single non-zero value to trigger the mask ROM path.
    std::vector<uint8_t> rom(0x8000, 0);
    rom[0x2000] = 1;

    if (mode == "correct") {
        const char *res = check_secure_area(rom);
        std::cout << res << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        long long dummy = 0;
        auto t1 = clock::now();
        for (long long i = 0; i < repeat; ++i)
            dummy += check_secure_area(rom)[0];
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 123456789) std::cerr << "";
    }

    return 0;
}

