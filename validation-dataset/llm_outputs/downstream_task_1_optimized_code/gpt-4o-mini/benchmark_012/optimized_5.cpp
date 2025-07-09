#include <vector>
#include <string>

struct DialogueToken {
    int type;
    size_t length;
};

struct MockTokenizer {
    bool karaoke;
    std::vector<int> setup_cost;

    explicit MockTokenizer(bool kt) : karaoke(kt), setup_cost(kt ? 1000 : 500, kt ? 1 : 0) {}

    std::vector<DialogueToken> tokenize(const std::string& str) const {
        std::vector<DialogueToken> out;
        out.reserve((str.size() / 10) + 1); // Preallocate memory
        size_t len = str.size() / 10 + 1;
        for (size_t i = 0; i < str.size(); i += len)
            out.push_back({karaoke ? 1 : 0, len});
        return out;
    }
};

std::vector<DialogueToken> tokenize(const std::string& str, bool karaoke_templater) {
    static MockTokenizer tokenizer(karaoke_templater); // Use static to avoid recreation
    return tokenizer.tokenize(str);
}

// Explicit template instantiation statements
