#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <string>
#include <numeric>

#define WORDS_BIGENDIAN 1

static const int CHUNK_SIZE = 4096;
static const int buffered_chunks = 4;

struct Buffer {
    std::vector<uint8_t> chunks;
    std::vector<int> chunkSize;
    int end = 0;
    int wrap = 0;
    Buffer() : chunks(buffered_chunks * CHUNK_SIZE), chunkSize(buffered_chunks) {}
};
struct AudioFormat { int bits; };
struct DecoderControl { bool stop = false; bool seek = false; double seekWhere = 0; };

int decode(Buffer* cb, AudioFormat* af, DecoderControl* dc,
           std::vector<uint16_t>& samples);

std::vector<uint16_t> generate_samples(size_t count) {
    std::vector<uint16_t> data(count);
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(0, 65535);
    for (auto& v : data) v = static_cast<uint16_t>(dist(gen));
    return data;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 8000;
    size_t samples = 1'000'000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--size=",0)==0) samples = std::stoul(arg.substr(7));
    }

    auto data = generate_samples(samples);
    Buffer cb;
    AudioFormat af;
    DecoderControl dc;

    if (mode == "correct") {
        decode(&cb, &af, &dc, data);
        long total = std::accumulate(cb.chunkSize.begin(), cb.chunkSize.end(), 0L);
        std::cout << total << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            cb.end = cb.wrap = 0;
            decode(&cb, &af, &dc, data);
        }
        auto t2 = clock::now();

        std::cout << "Time:  "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}
