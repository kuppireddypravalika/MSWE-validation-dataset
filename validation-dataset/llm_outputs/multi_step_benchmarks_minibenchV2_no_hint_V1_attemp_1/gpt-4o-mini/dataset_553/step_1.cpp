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
    const unsigned BUFFER_SIZE = MP3DecodeData::BUFFER_SIZE;
    const unsigned channels = data.frame.header.channels;
    const unsigned samples_per_iteration = BUFFER_SIZE / (2 * channels);

    while (i < pcm_length) {
        unsigned num_samples = std::min(samples_per_iteration, pcm_length - i);
        unsigned end = i + num_samples;

        for (unsigned j = i; j < end; j += channels) {
            for (unsigned ch = 0; ch < channels; ++ch) {
                acc += data.outputBuffer[j % BUFFER_SIZE];
            }
        }
        i = end;
    }
    return acc;
}