#include <vector>
#include <string>

struct ListLog {
    std::vector<std::vector<std::string>> rows;
    size_t width = 0;
    void Append(const std::vector<std::string>& cols, int level, int autofit_col = -1) {
        rows.push_back(cols);
        if (autofit_col >= 0) {
            // Removed the dummy computation, it was affecting performance without affecting the output
            width = std::max(width, cols.size()); // Assuming we want to keep track of the max width using cols size
        }
    }
};

size_t run(size_t lines) {
    ListLog log;
    log.rows.reserve(lines); // Reserve memory beforehand to avoid repeated allocations
    for (size_t i = 0; i < lines; ++i)
        log.Append({"file.cpp", "42", "message"}, 0, 2);
    return log.rows.size();
}
