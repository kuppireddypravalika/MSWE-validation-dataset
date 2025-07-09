#include <string>
#include <regex>
#include <optional>

struct ParsedURL {
    std::string service;
    unsigned version;
    std::string profile;
    std::string query;
};

std::optional<ParsedURL> parseURL(const std::string &url) {
    std::smatch match;
    static const std::regex parser("^/([a-zA-Z]+)/v([0-9]+)/([a-zA-Z]+)/(.+)$");
    if (std::regex_match(url, match, parser)) {
        ParsedURL out{match[1].str(), static_cast<unsigned>(std::stoul(match[2].str())), match[3].str(), match[4].str()};
        return out;
    }
    return std::nullopt;
}

// Explicit template instantiation statements
