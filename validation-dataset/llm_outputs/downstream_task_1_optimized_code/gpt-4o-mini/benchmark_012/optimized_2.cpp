#include <vector>
#include <string>

struct DialogueToken {
    int type;
    size_t length;
};

class Tokenizer {
public:
    explicit Tokenizer(bool karaoke) : karaoke_(karaoke) {}

    std::vector<DialogueToken> tokenize(const std::string& str) const {
        size_t len = str.size() / 10 + 1;
        size_t token_count = str.size() / len + (str.size() % len != 0);
        std::vector<DialogueToken> out;
        out.reserve(token_count);
        for (size_t i = 0; i < token_count; ++i) {
            out.push_back({karaoke_ ? 1 : 0, len});
        }
        return out;
    }

private:
    bool karaoke_;
};

std::vector<DialogueToken> tokenize(const std::string& str, bool karaoke_templater) {
    static Tokenizer tokenizer(karaoke_templater); // Create one instance per karaoke_templater value
    return tokenizer.tokenize(str);
}

// Explicit template instantiation statements
