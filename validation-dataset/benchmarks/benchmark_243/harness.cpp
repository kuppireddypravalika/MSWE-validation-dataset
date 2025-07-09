#include <chrono>
#include <iostream>
#include <string>
#include "ByteArray.h"

void ba_copy(const char* buf, size_t len, ByteArray& data);

static int checksum(const ByteArray& b)
{
    int sum = 0;
    sum = b.checksum();
    return sum;
}

int main(int argc, char* argv[])
{
    std::string mode = "perf";
    size_t size = 1 << 22; // 4MB
    int repeat = 20000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--size=", 0) == 0)
            size = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    std::string buffer(size, 'x');
    ByteArray data;
    data.assign(buffer.data(), size); // preallocate

    if (mode == "correct") {
        ba_copy(buffer.data(), buffer.size(), data);
        std::cout << checksum(data) << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;

        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            ba_copy(buffer.data(), buffer.size(), data);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
