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

    double result = 0.0;
    time_t current_time = start + static_cast<time_t>((size / 2) * 60);
    struct std::tm tmp;
    std::vector<time_t>::iterator it;

    for (int r = 0; r < repeat; ++r) {
        for (it = steps.begin(); it != steps.end(); ++it) {
            localtime_r(&(*it), &tmp);             // fromTime_t equivalent
            if (*it > current_time) {
                result += values[std::distance(steps.begin(), it)];
                break;
            }
        }
    }
    return result;
}