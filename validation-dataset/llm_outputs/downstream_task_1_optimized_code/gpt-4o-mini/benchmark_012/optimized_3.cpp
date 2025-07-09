#include <vector>
#include <string>

struct DialogueToken {
    int type;
    size_t length;
};

class Tokenizer {
public:
    Tokenizer(bool kt) : karaoke(kt) {}
    std::vector<DialogueToken> tokenize(const std::string& str) const {
        size_t len = str.size() / 10 + 1;
        size_t num_tokens = (str.size() + len - 1) / len;  // ceiling division
        std::vector<DialogueToken> out;
        out.reserve(num_tokens);  // reservation to avoid multiple reallocations
        for (size_t i = 0; i < num_tokens; ++i) {
            out.push_back({karaoke ? 1 : 0, len});
        }
        return out;
    }
private:
    bool karaoke;
};

std::vector<DialogueToken> tokenize(const std::string& str, bool karaoke_templater) {
    Tokenizer tokenizer(karaoke_templater);
    return tokenizer.tokenize(str);
}

// Explicit template instantiation statements (if any) go here.