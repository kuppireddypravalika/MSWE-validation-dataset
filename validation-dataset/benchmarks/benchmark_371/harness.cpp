#include "common.hpp"
#include <chrono>
#include <iostream>
#include <string>

ListInit* convert_list(const ListInit* li, RecTy* ty);

// Create a list with sequential integers
static ListInit* createList(size_t size, ListRecTy* ty) {
    std::vector<Init*> elems;
    elems.reserve(size);
    for (size_t i = 0; i < size; ++i)
        elems.push_back(new IntInit(static_cast<int>(i)));
    return ListInit::get(elems, ty);
}

// Fill the global pool with many unrelated lists to make lookups costly
static void populatePool(size_t count, size_t size, ListRecTy* ty) {
    for (size_t c = 0; c < count; ++c) {
        std::vector<Init*> elems;
        elems.reserve(size);
        for (size_t i = 0; i < size; ++i)
            elems.push_back(new IntInit(static_cast<int>(c + i)));
        ListInit::get(elems, ty);
    }
}

static long long sumList(const ListInit* li) {
    long long sum = 0;
    for (Init* I : li->getValues())
        sum += static_cast<IntInit*>(I)->value;
    return sum;
}

int main(int argc, char** argv) {
    std::string mode = "perf";
    int repeat = 90000;
    size_t size = 100;
    size_t pool = 10000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--size=", 0) == 0)
            size = std::stoul(arg.substr(7));
        else if (arg.rfind("--pool=", 0) == 0)
            pool = std::stoul(arg.substr(7));
    }

    IntRecTy intTy;
    ListRecTy listTy(&intTy);

    populatePool(pool, size, &listTy);
    ListInit* li = createList(size, &listTy);

    if (mode == "correct") {
        ListInit* out = convert_list(li, &listTy);
        std::cout << sumList(out) << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            li = convert_list(li, &listTy);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
}
