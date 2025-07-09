#include <chrono>
#include <iostream>
#include <string>
#include <vector>

// declaration of the function under test
void convertFromAscii(const char* a, int len, char16_t*& out);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t size = 8192;     // number of input bytes
    int repeat = 1200000;        // iterations for performance timing

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--size=", 0) == 0)
            size = static_cast<std::size_t>(std::stoul(arg.substr(7)));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    std::string input(size, 'A');
    std::vector<char16_t> buffer(size);

    if (mode == "correct") {
        char16_t* out = buffer.data();
        convertFromAscii(input.c_str(), static_cast<int>(size), out);
        long long sum = 0;
        for (char16_t ch : buffer) sum += ch;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        long long dummy = 0;

        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            char16_t* out = buffer.data();
            convertFromAscii(input.c_str(), static_cast<int>(size), out);
            dummy += buffer[0];
        }
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 123456789) std::cerr << "";
    }
    return 0;
}
