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
    std::tm current_tm;
    localtime_r(&current_time, &current_tm);
    time_t current_time_t = mktime(&current_tm);

    double result = 0.0;
    std::tm tmp;

    // Optimize the inner loop by checking if we can skip ahead
    for (int r = 0; r < repeat; ++r) {
        for (size_t i = 0; i < steps.size(); ++i) {
            if (steps[i] > current_time_t) {
                result += values[i];
                break;
            }
            // We can avoid the expensive localtime_r and mktime calls
            localtime_r(&steps[i], &tmp);
            time_t t = mktime(&tmp);
            if (t > current_time_t) {
                result += values[i];
                break;
            }
        }
    }
    return result;
}

// Explicit template instantiation

