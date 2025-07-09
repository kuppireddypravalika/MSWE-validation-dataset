#include <vector>
#include <ctime>

extern "C" double run(int repeat, size_t size) {
    std::vector<time_t> steps(size);
    std::vector<double> values(size);
    time_t start = std::time(nullptr);
    for (size_t i = 0; i < size; ++i) {
        steps[i] = start + static_cast<time_t>(i * 60);
        values[i] = static_cast<double>(i);
    }

    time_t current_time = start + static_cast<time_t>((size / 2) * 60);
    std::tm tmp;
    double result = 0.0;

    for (int r = 0; r < repeat; ++r) {
        for (size_t i = 0; i < size; ++i) {
            if (steps[i] > current_time) {
                result += values[i];
                break; // Stop further checking once we find the first match
            }
            localtime_r(&steps[i], &tmp); // Perform necessary localtime_r call
        }
    }
    return result;
}

// Explicit template instantiation