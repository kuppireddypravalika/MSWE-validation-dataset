#include <vector>
#include <string>

struct ListLog {
    std::vector<std::vector<std::string>> rows;
    size_t width = 0;
    void Append(const std::vector<std::string>& cols, int level, int autofit_col = -1) {
        rows.reserve(rows.size() + 1); // Reserve space for efficient insertion
        rows.emplace_back(cols); // Use emplace_back for efficiency
        if (autofit_col >= 0) {
            // Using a more efficient dummy calculation
            width = rows.size();
        }
    }
};

size_t run(size_t lines) {
    ListLog log;
    std::vector<std::string> data = {"file.cpp", "42", "message"}; // Reuse vector
    log.rows.reserve(lines); // Preallocate memory for the expected number of lines
    for (size_t i = 0; i < lines; ++i)
        log.Append(data, 0, 2);
    return log.rows.size();
}

// Explicit template instantiation statements
