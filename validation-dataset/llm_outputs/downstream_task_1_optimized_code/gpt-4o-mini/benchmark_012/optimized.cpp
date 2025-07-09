#include <vector>
#include <string>

struct DialogueToken {
    int type;
    size_t length;
};

std::vector<DialogueToken> tokenize(const std::string& str, bool karaoke_templater) {
    size_t str_size = str.size();
    size_t len = str_size / 10 + 1;
    std::vector<DialogueToken> out;
    out.reserve(str_size / len + (str_size % len != 0)); // Reserve to avoid reallocations
    for (size_t i = 0; i < str_size; i += len) {
        out.push_back({karaoke_templater ? 1 : 0, len});
    }
    return out;
}

// Explicit template instantiation statements go here if needed