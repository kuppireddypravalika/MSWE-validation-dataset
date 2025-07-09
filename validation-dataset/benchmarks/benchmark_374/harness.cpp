#include <chrono>
#include <iostream>
#include <string>
#include <vector>

struct TemplateParameterList {
    int min_required;
    int total_size;
    TemplateParameterList(int req = 1, int size = 1) : min_required(req), total_size(size) {}
    int getMinRequiredArguments() const {
        volatile int x = 0;
        for (int i = 0; i < 1000; ++i) x += i;
        return min_required;
    }
    int size() const { return total_size; }
};

struct Decl {
    TemplateParameterList tpl;
    Decl(int req = 1, int sz = 1) : tpl(req, sz) {}
    TemplateParameterList* getTemplateParameters() const { return const_cast<TemplateParameterList*>(&tpl); }
};

size_t find_with_defaults(const std::vector<Decl>& chain, int repeat);

std::vector<Decl> create_chain(size_t n, size_t first_default) {
    std::vector<Decl> chain;
    chain.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        if (i == first_default)
            chain.emplace_back(0, 1); // has default
        else
            chain.emplace_back(1, 1); // no default
    }
    return chain;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t decls = 5000;
    size_t first_default = 1;
    int repeat = 5000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--decls=", 0) == 0) decls = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--first=", 0) == 0) first_default = std::stoul(arg.substr(8));
    }

    std::vector<Decl> chain = create_chain(decls, first_default);

    if (mode == "correct") {
        size_t result = find_with_defaults(chain, 1);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        size_t res = find_with_defaults(chain, repeat);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        (void)res;
    }
}
