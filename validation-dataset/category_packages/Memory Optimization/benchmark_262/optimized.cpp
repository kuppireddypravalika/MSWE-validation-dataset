#include <vector>
#include <cstdint>
#include <cstring>
#include <algorithm>

#define WORDS_BIGENDIAN 1

#ifdef WORDS_BIGENDIAN
#define OGG_DECODE_USE_BIGENDIAN 1
#else
#define OGG_DECODE_USE_BIGENDIAN 0
#endif

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

struct DecoderControl {
    bool stop = false;
    bool seek = false;
    double seekWhere = 0;
};

struct OggVorbis_File {
    std::vector<uint16_t>* data;
    size_t pos;
};

static long ov_read(OggVorbis_File* vf, char* ptr, int length,
             int bigendianp, int word, int sgn, int* current_section) {
    (void)current_section; (void)word; (void)sgn;
    size_t remaining = (vf->data->size() - vf->pos) * sizeof(uint16_t);
    if (remaining == 0) return 0;
    size_t to_copy = std::min<size_t>(remaining, (size_t)length);
    const uint16_t* src = vf->data->data() + vf->pos;
    vf->pos += to_copy / 2;
    if (bigendianp) {
        for (size_t i = 0; i < to_copy / 2; ++i) {
            uint16_t val = src[i];
            ptr[2*i] = static_cast<char>(val >> 8);
            ptr[2*i+1] = static_cast<char>(val & 0xFF);
        }
    } else {
        std::memcpy(ptr, src, to_copy);
    }
    return to_copy;
}

int decode(Buffer* cb, AudioFormat* af, DecoderControl* dc,
           std::vector<uint16_t>& samples) {
    OggVorbis_File vf{&samples, 0};
    af->bits = 16;
    int current_section = 0;
    int eof = 0;
    long ret;
    char chunk[CHUNK_SIZE];
    int chunkpos = 0;
    while (!eof) {
        ret = ov_read(&vf, chunk + chunkpos, CHUNK_SIZE - chunkpos,
                      OGG_DECODE_USE_BIGENDIAN, 2, 1, &current_section);
        if (ret <= 0) eof = 1; else chunkpos += ret;
        if (chunkpos >= CHUNK_SIZE || eof) {
            if (dc->stop) break;
            else if (dc->seek) continue;
            std::memcpy(&cb->chunks[cb->end * CHUNK_SIZE], chunk, chunkpos);
            cb->chunkSize[cb->end] = chunkpos;
            chunkpos = 0;
            cb->end++;
            if (cb->end >= buffered_chunks) {
                cb->end = 0;
                cb->wrap = 1;
            }
        }
    }
    return 0;
}
