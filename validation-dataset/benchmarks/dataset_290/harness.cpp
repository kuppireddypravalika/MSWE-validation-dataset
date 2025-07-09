#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

constexpr int nLeaderSize = 24;
constexpr char DDF_FIELD_TERMINATOR = 0x1e;

int read_record(FILE* fp, int recLength);

std::vector<char> make_record(size_t data_size, size_t extra) {
    std::vector<char> buf(nLeaderSize + data_size + extra, 'A');
    buf[buf.size() - 2] = DDF_FIELD_TERMINATOR;
    buf[buf.size() - 1] = DDF_FIELD_TERMINATOR;
    return buf;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t size = 50;
    size_t extra = 200;
    int repeat = 2500000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=",0)==0) size = std::stoul(arg.substr(7));
        else if (arg.rfind("--extra=",0)==0) extra = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    auto buf = make_record(size, extra);
    FILE* fp = fmemopen(buf.data(), buf.size(), "rb");
    if (!fp) return 1;

    if (mode == "correct") {
        fseek(fp, 0, SEEK_SET);
        int result = read_record(fp, static_cast<int>(size + nLeaderSize));
        std::cout << result << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        long long dummy = 0;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            fseek(fp, 0, SEEK_SET);
            dummy += read_record(fp, static_cast<int>(size + nLeaderSize));
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 123456789) std::cerr << "";
    }

    fclose(fp);
    return 0;
}
