#include <string>
#include <vector>
#include <cctype>
#include <cstdint>

namespace {

enum ParseOption : uint32_t {
    CacheCompletionResults = 1u << 0,
    PrecompiledPreamble   = 1u << 1,
    SkipFunctionBodies    = 1u << 2,
    LimitSkipToPreamble   = 1u << 3,
    IncludeBriefComments  = 1u << 4,
    DetailedPreprocess    = 1u << 5,
    KeepGoing             = 1u << 6
};

uint32_t defaultParseOptions() {
    return CacheCompletionResults |
           PrecompiledPreamble |
           IncludeBriefComments |
           DetailedPreprocess |
           KeepGoing;
}

size_t parseSource(const std::string& src, uint32_t options) {
    const bool skip = options & SkipFunctionBodies;
    std::vector<std::string> tokens;
    std::string cur;
    bool inFunc = false;
    size_t depth = 0;
    tokens.reserve(src.size() / 2); // Reserve memory for tokens to optimize allocations

    for (size_t i = 0; i < src.size(); ++i) {
        char c = src[i];
        if (c == '{') {
            if (!inFunc) {
                inFunc = true;
                if (skip) {
                    depth = 1;
                    ++i;
                    while (i < src.size() && depth > 0) {
                        if (src[i] == '{') ++depth;
                        else if (src[i] == '}') --depth;
                        ++i;
                    }
                    inFunc = false;
                    if (!cur.empty()) {
                        tokens.push_back(std::move(cur)); // Use move semantics
                        cur.clear();
                    }
                    continue;
                }
            } else {
                ++depth;
            }
        } else if (c == '}' && inFunc) {
            if (depth == 0) inFunc = false;
            else --depth;
        }

        bool record = !inFunc; // collect tokens only outside function bodies
        if (!std::isalnum(static_cast<unsigned char>(c))) {
            if (!cur.empty() && record) {
                tokens.push_back(std::move(cur)); // Use move semantics
            }
            cur.clear();
        } else {
            cur += c;
        }
    }
    if (!cur.empty() && !inFunc)
        tokens.push_back(std::move(cur)); // Use move semantics

    size_t sum = 0;
    for (const auto& t : tokens) sum += t.size();
    return sum;
}

} // namespace

size_t parse_source(const std::string& src) {
    return parseSource(src, defaultParseOptions());
}

// Explicit template instantiation statements, if present in the original code, go here.
