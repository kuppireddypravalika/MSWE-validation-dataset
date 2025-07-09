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
    using It = std::string::const_iterator;
    It begin = url.begin();
    It end = url.end();
    static const std::regex parser("^/([a-zA-Z]+)/v([0-9]+)/([a-zA-Z]+)/(.+)$");
    std::match_results<It> match;
    if (std::regex_match(begin, end, match, parser)) {
        ParsedURL out{match[1], static_cast<unsigned>(std::stoul(match[2])), match[3], match[4]};
        return out;
    }
    return std::nullopt;
}
