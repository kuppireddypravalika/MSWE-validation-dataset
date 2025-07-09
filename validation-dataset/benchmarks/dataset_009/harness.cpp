#include <chrono>
#include <iostream>
#include <random>
#include <string>

void search_replace(std::string& text, const std::string& find, const std::string& replace);

static std::string generate_text(size_t groups) {
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist('a', 'z');
    std::string text;
    text.reserve(groups * 24); // include filler characters
    for (size_t i = 0; i < groups; ++i) {
        text += "abc";
        if (i + 1 < groups) {
            for (int j = 0; j < 20; ++j) text.push_back(static_cast<char>(dist(gen)));
        }
    }
    return text;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t size = 5000;
    int repeat = 100;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=",0)==0) size = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    std::string base = generate_text(size);
    const std::string pattern = "abc";
    const std::string repl = "XYZ";

    if (mode == "correct") {
        std::string text = base;
        search_replace(text, pattern, repl);
        long long sum = 0;
        for (char c : text) sum += c;
        std::cout << sum << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        long long dummy = 0;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            std::string text = base;
            search_replace(text, pattern, repl);
            dummy += text.size();
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 42) std::cerr << ""; // prevent optimization
    }
    return 0;
}
