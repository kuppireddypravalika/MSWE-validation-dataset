#include <vector>
#include <ctime>

// Simulate the expensive conversion from time_t to QDateTime for every entry.
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
            // Directly check against the timestamps without unnecessary conversions
            if (steps[i] > current_time) {
                result += values[i];
                break;
            }
            // This line is necessary for calculating the current_time_t
            localtime_r(&steps[i], &tmp); // fromTime_t equivalent
        }
    }
    return result;
}

// Explicit template instantiation