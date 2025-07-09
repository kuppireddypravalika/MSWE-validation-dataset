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
    static const std::regex parser("^/([a-zA-Z]+)/v([0-9]+)/([a-zA-Z]+)/(.+)$");
    std::match_results<std::string::const_iterator> match;
    if (std::regex_match(url, match, parser)) {
        return ParsedURL{match[1].str(), static_cast<unsigned>(std::stoul(match[2].str())), match[3].str(), match[4].str()};
    }
    return std::nullopt;
}

// Explicit template instantiation statements (if any) would go here.
