#include <chrono>
#include <iostream>
#include <string>
#include <vector>

size_t process(const std::vector<std::string>& hrefs,
               const std::string& newUrl,
               bool ignoreHidden);

std::vector<std::string> createData(size_t n) {
    std::vector<std::string> vec;
    vec.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        vec.push_back("/path/" + std::to_string(i) + ".txt");
    }
    // add a hidden file for regex to match
    vec.push_back("/path/.hidden");
    return vec;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";        // perf or correct
    size_t entries = 1000;
    int repeat = 8200;               // workload
    bool ignoreHidden = true;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--entries=", 0) == 0)
            entries = std::stoul(arg.substr(10));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    std::vector<std::string> data = createData(entries);
    const std::string target = "/path/42.txt";

    if (mode == "correct") {
        size_t result = process(data, target, ignoreHidden);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            process(data, target, ignoreHidden);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
}
