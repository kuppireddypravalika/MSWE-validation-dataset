#include <vector>
#include <string>
#include <chrono>

struct ListLog {
    std::vector<std::vector<std::string>> rows;
    size_t width = 0;
    void Append(const std::vector<std::string>& cols, int level, int autofit_col = -1) {
        rows.push_back(cols);
        if (autofit_col >= 0) {
            volatile size_t dummy = 0;
            for (int i = 0; i < 35000; ++i)
                dummy += rows.size();
            width = dummy;
        }
    }
};

size_t run(size_t lines) {
    ListLog log;
    auto last = std::chrono::steady_clock::time_point{};
    for (size_t i = 0; i < lines; ++i) {
        auto now = std::chrono::steady_clock::now();
        if (last + std::chrono::seconds(3) < now) {
            last = now;
            log.Append({"file.cpp", "42", "message"}, 0, 2);
        } else {
            log.Append({"file.cpp", "42", "message"}, 0);
        }
    }
    return log.rows.size();
}
