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
    }
    for (int i = 0; i < *num; ++i) {
        (*out)[i] = clip->boxes[i];
    }
    volatile int sink = 0;
    for (int i = 0; i < *num; ++i)
        for (int j = 0; j < HEAVY_ITERS; ++j)
            sink += (*out)[i].x1 + j;
    return sink;
}

int mask_function(const Clip& clip, const Mask& mask) {
    Box stack_box{}; 
    Box* boxes = &stack_box;
    int num = 0;
    // Only perform heavy work when mask is solid alpha
    if (mask.solid) {
        heavy_clip_get_boxes(&clip, &boxes, &num);
        if (boxes != &stack_box) delete[] boxes;
    }
    return static_cast<int>(clip.boxes.size()) + (mask.solid ? 1 : 0);
}
