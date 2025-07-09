#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <cstdint>

namespace bench109 {
struct MemData;
std::vector<std::vector<int8_t>> getMemoryPages(const MemData&, bool, uint64_t, uint64_t);
}

using namespace bench109;

namespace bench109 {
enum { ChunkBits = 10 };
enum { PageBits = 10 };
enum { PageSize = 1 << PageBits };
enum { PageChunkBits = ChunkBits + PageBits };
enum { PageChunkSize = 1 << PageChunkBits };
enum { PageChunkMask = PageChunkSize - 1 };

struct Alloc {
    uint64_t ptr;
    uint64_t size;
    int64_t timeAlloc;
    int64_t timeFree;
};

struct MemData {
    std::vector<Alloc> data;
    uint64_t low;
    uint64_t high;
};
}

MemData createMemData(size_t allocs, std::mt19937& gen)
{
    MemData mem;
    mem.data.resize(allocs);
    uint64_t ptr = 0;
    std::uniform_int_distribution<uint64_t> sizeDist(1, 1);
    for (size_t i = 0; i < allocs; ++i) {
        uint64_t size = sizeDist(gen);
        mem.data[i].ptr = ptr;
        mem.data[i].size = size;
        mem.data[i].timeAlloc = static_cast<int64_t>(i);
        mem.data[i].timeFree = -1; // active
        ptr += size + 16; // some gap
    }
    mem.low = 0;
    mem.high = ptr + PageChunkSize;
    return mem;
}

int main(int argc, char* argv[])
{
    std::string mode = "perf";
    size_t allocs = 50000;
    int repeat = 35000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--allocs=", 0) == 0) allocs = std::stoull(arg.substr(9));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    std::mt19937 gen(42);
    MemData mem = createMemData(allocs, gen);

    if (mode == "correct") {
        auto pages = getMemoryPages(mem, false, 0, 100);
        // output sum to avoid optimizing away
        size_t sum = 0;
        for (const auto& p : pages) sum += p.size();
        std::cout << sum << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            auto pages = getMemoryPages(mem, false, 0, 100);
            (void)pages;
        }
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
