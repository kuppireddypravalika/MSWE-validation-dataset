#include <iostream>
#include <vector>
#include <string>
#include <chrono>

struct DialogueToken {
    int type;
    size_t length;
};

std::vector<DialogueToken> tokenize(const std::string&, bool);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 25000000;
    size_t length = 20000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--size=", 0) == 0) length = std::stoul(arg.substr(7));
    }

    std::string text(length, 'a');
    std::vector<DialogueToken> result;

    if (mode == "correct") {
        result = tokenize(text, true);
        for (const auto& tok : result) std::cout << tok.type << ":" << tok.length << " ";
        std::cout << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;

        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            result = tokenize(text, true);
        }
        auto t2 = clock::now();

        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }

    return 0;
}
