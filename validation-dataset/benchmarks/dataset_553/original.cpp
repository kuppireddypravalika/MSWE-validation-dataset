#include <cstdint>
#include <cstddef>
#include <algorithm>

struct MP3FrameHeader { volatile unsigned channels; };
struct MP3Frame { MP3FrameHeader header; };
struct MP3DecodeData {
    static constexpr std::size_t BUFFER_SIZE = 64;
    int16_t outputBuffer[BUFFER_SIZE];
    MP3Frame frame;
};

__attribute__((noinline)) unsigned MAD_NCHANNELS(const MP3FrameHeader* h) { return h->channels; }

long long process(MP3DecodeData& data, unsigned pcm_length) {
    unsigned i = 0;
    long long acc = 0;
    while (i < pcm_length) {
        unsigned num_samples = sizeof(data.outputBuffer) /
            (2 * MAD_NCHANNELS(&data.frame.header));
        if (num_samples > pcm_length - i)
            num_samples = pcm_length - i;
        for (unsigned j = 0; j < num_samples * data.frame.header.channels; ++j) {
            unsigned idx = j % MP3DecodeData::BUFFER_SIZE;
            acc += data.outputBuffer[idx];
        }
        i += num_samples;
    }
    return acc;
}

