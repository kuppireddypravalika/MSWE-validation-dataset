#include <chrono>
#include <iostream>
#include <string>
#include <vector>

struct ExpectedStr {
    bool valid;
    std::string value;
    std::string err;
};

ExpectedStr getName(const std::string &table, size_t index);
size_t process(const std::string &table, const std::vector<size_t> &indices, int repeat);

std::vector<size_t> buildStringTable(size_t num, std::string &out) {
    std::vector<size_t> indices;
    out.clear();
    for (size_t i = 0; i < num; ++i) {
        indices.push_back(out.size());
        out += "sym" + std::to_string(i);
        out.push_back('\0');
    }
    return indices;
}

int main(int argc, char **argv) {
    std::string mode = "perf";
    size_t num_syms = 2000;
    int repeat = 110000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--syms=", 0) == 0) num_syms = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    std::string table;
    std::vector<size_t> indices = buildStringTable(num_syms, table);

    if (mode == "correct") {
        size_t result = process(table, indices, 1);
        std::cout << result << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        process(table, indices, repeat);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}
