#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

struct Attribute {
    uint32_t kind;
    uint32_t value;
};

unsigned long sort_attributes(const std::vector<Attribute>& attrs);

std::vector<Attribute> createAttrs(size_t n) {
    std::mt19937 rng(12345);
    std::uniform_int_distribution<uint32_t> dist(0, 65535);
    std::vector<Attribute> v(n);
    for (size_t i = 0; i < n; ++i) {
        v[i].kind = dist(rng);
        v[i].value = dist(rng);
    }
    return v;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t elems = 70000;
    int repeat = 1200;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--elems=",0)==0)
            elems = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=",0)==0)
            repeat = std::stoi(arg.substr(9));
    }

    std::vector<Attribute> data = createAttrs(elems);

    if (mode == "correct") {
        unsigned long result = sort_attributes(data);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r=0;r<repeat;++r)
            sort_attributes(data);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
                  << " ms\n";
    }
}
