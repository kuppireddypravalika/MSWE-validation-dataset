#include <chrono>
#include <iostream>
#include <string>
#include <random>

struct MP3FrameHeader { volatile unsigned channels; };
struct MP3Frame { MP3FrameHeader header; };
struct MP3DecodeData {
    static constexpr std::size_t BUFFER_SIZE = 64;
    int16_t outputBuffer[BUFFER_SIZE];
    MP3Frame frame;
};

long long process(MP3DecodeData& data, unsigned pcm_length);
long long process(MP3DecodeData& data, unsigned pcm_length);

MP3DecodeData make_data(unsigned channels) {
    MP3DecodeData d;
    d.frame.header.channels = channels;
    std::mt19937 gen(42);
    std::uniform_int_distribution<int16_t> dist(0, 32767);
    for (std::size_t i = 0; i < MP3DecodeData::BUFFER_SIZE; ++i)
        d.outputBuffer[i] = dist(gen);
    return d;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::string impl = "original";
    unsigned pcm_length = 1 << 18; // default 256k
    int repeat = 33000; // ensure original runtime >= 10s

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg == "--impl=optimized") impl = "optimized";
        else if (arg == "--impl=original") impl = "original";
        else if (arg.rfind("--pcm=",0)==0) pcm_length = static_cast<unsigned>(std::stoul(arg.substr(6)));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    MP3DecodeData base = make_data(2);
    long long result = 0;
    if (mode == "correct") {
        MP3DecodeData data = base;
        if (impl == "optimized")
            result = process(data, pcm_length);
        else
            result = process(data, pcm_length);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            MP3DecodeData data = base;
            if (impl == "optimized")
                result += process(data, pcm_length);
            else
                result += process(data, pcm_length);
        }
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }
    return 0;
}

