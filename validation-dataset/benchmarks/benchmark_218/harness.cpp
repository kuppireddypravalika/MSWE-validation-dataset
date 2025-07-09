#include <chrono>
#include <iostream>
#include <string>
#include <vector>

struct APInt {
    std::vector<uint64_t> data;
    unsigned num_words;
    APInt(unsigned total, unsigned valid) : data(total, 0), num_words(valid) {}
    unsigned getNumWords() const { return num_words; }
    const uint64_t* getRawData() const { return data.data(); }
};

struct FltSemantics { unsigned precision; };

struct APFloat {
    const FltSemantics* semantics;
    bool sign = false;
    unsigned category = 0;
    std::vector<uint64_t> significand;
    APFloat(const FltSemantics& sem, unsigned parts)
        : semantics(&sem), significand(parts, 0) {}
    unsigned partCount() const { return significand.size(); }
};

void makeNaN(APFloat& f, bool SNaN, bool Negative, const APInt& fill);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    unsigned parts = 4000000;
    unsigned valid = 1;
    int repeat = 2000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--parts=",0)==0) parts = std::stoul(arg.substr(8));
        else if (arg.rfind("--valid=",0)==0) valid = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    FltSemantics sem{parts * 64};
    APFloat value(sem, parts);
    APInt payload(parts, valid);
    for (unsigned i = 0; i < parts; ++i) payload.data[i] = (i < valid) ? 42 : 0;

    if (mode == "correct") {
        makeNaN(value, false, false, payload);
        double sum = 0.0;
        for (uint64_t v : value.significand) sum += static_cast<double>(v);
        std::cout << sum << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i=0;i<repeat;++i) makeNaN(value, false, false, payload);
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << " ms\n";
    }
    return 0;
}
