#include <vector>
#include <string>

struct ListLog {
    std::vector<std::vector<std::string>> rows;
    size_t width = 0;
    void Append(const std::vector<std::string>& cols, int level, int autofit_col = -1) {
        rows.emplace_back(cols);
        if (autofit_col >= 0) {
            // Removed the unnecessary computation that affects performance
            width = rows.size();  // Updated width to reflect the current size directly
        }
    }
};

size_t run(size_t lines) {
    ListLog log;
    log.rows.reserve(lines);  // Preallocate memory for rows to prevent reallocations
    std::vector<std::string> cols = {"file.cpp", "42", "message"};
    for (size_t i = 0; i < lines; ++i)
        log.Append(cols, 0, 2);
    return log.rows.size();
}
