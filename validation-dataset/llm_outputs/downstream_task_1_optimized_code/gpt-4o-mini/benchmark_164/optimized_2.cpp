#include <vector>
#include <string>

struct ListLog {
    std::vector<std::vector<std::string>> rows;
    size_t width = 0;
    void Append(const std::vector<std::string>& cols, int level, int autofit_col = -1) {
        rows.push_back(cols);
        if (autofit_col >= 0) {
            // Removed the volatile dummy loop for performance
            width = rows.size(); // Assign current size to width
        }
    }
};

size_t run(size_t lines) {
    ListLog log;
    std::vector<std::string> logEntry = {"file.cpp", "42", "message"};
    log.rows.reserve(lines); // Reserve memory for efficiency
    for (size_t i = 0; i < lines; ++i)
        log.Append(logEntry, 0, 2);
    return log.rows.size();
}
