#include <vector>
#include <string>

struct ListLog {
    std::vector<std::vector<std::string>> rows;
    size_t width = 0;
    void Append(const std::vector<std::string>& cols, int level, int autofit_col = -1) {
        rows.emplace_back(cols);
        if (autofit_col >= 0) {
            width = rows.size();
        }
    }
};

size_t run(size_t lines) {
    ListLog log;
    std::vector<std::string> message = {"file.cpp", "42", "message"};
    log.rows.reserve(lines);  // Reserve memory once to avoid reallocations
    for (size_t i = 0; i < lines; ++i)
        log.Append(message, 0, 2);
    return log.rows.size();
}

// Explicit template instantiation statements if needed.
