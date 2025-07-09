#include <vector>
#include <cstddef>

struct Box { int x1, y1, x2, y2; };
struct Clip { std::vector<Box> boxes; };
struct Mask { bool solid; };

static const int HEAVY_ITERS = 800000;

static int heavy_clip_get_boxes(const Clip* clip, Box** out, int* num) {
    *num = static_cast<int>(clip->boxes.size());
    if (*num > 1) {
        *out = new Box[*num];
        std::copy(clip->boxes.begin(), clip->boxes.end(), *out);
    } else {
        *out = nullptr;
    }
    volatile int sink = 0;
    for (int i = 0; i < *num; ++i)
        for (int j = 0; j < HEAVY_ITERS; ++j)
            sink += (*out)[i].x1 + j;
    return sink;
}

int mask_function(const Clip& clip, const Mask& mask) {
    Box* boxes = nullptr;
    int num = 0;
    // heavy work performed whenever a clip is supplied
    heavy_clip_get_boxes(&clip, &boxes, &num);
    if (num > 1) delete[] boxes;
    // output independent of heavy work
    return static_cast<int>(clip.boxes.size()) + (mask.solid ? 1 : 0);
}

// Explicit template instantiations
