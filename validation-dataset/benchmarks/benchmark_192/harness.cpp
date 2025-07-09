#include <chrono>
#include <iostream>
#include <string>
#include <optional>

struct ParsedURL {
    std::string service;
    unsigned version;
    std::string profile;
    std::string query;
};

std::optional<ParsedURL> parseURL(const std::string &url);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 120000;
    std::string url = "/route/v1/driving/7.416351,43.731205;7.420363,43.736189";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        std::optional<ParsedURL> result = parseURL(url);
        if (result)
            std::cout << result->service << ' ' << result->version << ' ' << result->profile << ' ' << result->query << '\n';
        else
            std::cout << "parse_fail\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            parseURL(url);
        }
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}
