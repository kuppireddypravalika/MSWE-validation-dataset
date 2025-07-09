#include "notification_area.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::string impl = "original"; // parsed but unused
    std::size_t count = 1000;
    long long repeat = 41000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--impl=",0)==0) impl = arg.substr(8);
        else if (arg.rfind("--count=",0)==0) count = static_cast<std::size_t>(std::stoul(arg.substr(8)));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoll(arg.substr(9));
    }

    NotificationArea area(count);
    std::vector<Point> positions;
    positions.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        Rect r = area.notificationRect(i);
        positions.push_back({r.x + r.w/2, r.y + r.h/2});
    }

    if (mode == "correct") {
        std::size_t sum = 0;
        for (std::size_t i = 0; i < count; ++i) sum += area.notificationIndex(positions[i]);
        std::cout << sum << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        std::size_t dummy = 0;
        auto t1 = clock::now();
        for (long long r = 0; r < repeat; ++r) {
            for (std::size_t i = 0; i < count; ++i) {
                dummy += area.notificationIndex(positions[i]);
            }
        }
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
        if (dummy == 123456789) std::cerr << "";
    }
    return 0;
}
