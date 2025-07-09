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

    // Loop over the number of repeats
    for (int r = 0; r < repeat; ++r) {
        // Optimize the inner loop by checking the condition before conversion
        for (size_t i = 0; i < size; ++i) {
            // Directly check against the timestamp to skip unnecessary conversions
            if (steps[i] > current_time) {
                result += values[i];
                break;  // Exit the loop as we only need the first match
            }
            localtime_r(&steps[i], &tmp);  // Perform necessary localtime_r call
        }
    }
    return result;
}

// Explicit template instantiation

