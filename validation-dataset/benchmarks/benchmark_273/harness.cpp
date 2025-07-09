#include <chrono>
#include <iostream>
#include <string>
#include <vector>

struct Box { int x1, y1, x2, y2; };
struct Clip { std::vector<Box> boxes; };
struct Mask { bool solid; };

int mask_function(const Clip& clip, const Mask& mask);

Clip create_clip(size_t count) {
    Clip c;
    c.boxes.resize(count);
    for (size_t i = 0; i < count; ++i) {
        c.boxes[i] = {static_cast<int>(i), static_cast<int>(i), static_cast<int>(i+1), static_cast<int>(i+1)};
    }
    return c;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t boxes = 1000;
    int repeat = 50;
    bool solid = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--boxes=",0) == 0) boxes = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=",0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg == "--solid") solid = true;
    }

    Clip clip = create_clip(boxes);
    Mask mask{solid};

    if (mode == "correct") {
        int res = mask_function(clip, mask);
        std::cout << res << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    for (int i = 0; i < repeat; ++i) mask_function(clip, mask);
    auto t2 = clock::now();

    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    return 0;
}
