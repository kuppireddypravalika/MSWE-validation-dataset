#include <string>
#include <regex>
#include <optional>
#include <stdexcept>

struct ParsedURL {
    std::string service;
    unsigned version;
    std::string profile;
    std::string query;
};

std::optional<ParsedURL> parseURL(const std::string &url) {
    static const std::regex parser("^/([a-zA-Z]+)/v([0-9]+)/([a-zA-Z]+)/(.+)$");
    std::smatch match;
    if (std::regex_match(url, match, parser)) {
        try {
            return ParsedURL{match[1].str(), static_cast<unsigned>(std::stoul(match[2].str())), match[3].str(), match[4].str()};
        } catch (const std::invalid_argument &e) {
            return std::nullopt;
        } catch (const std::out_of_range &e) {
            return std::nullopt;
        }
    }
    return std::nullopt;
}

// Explicit template instantiation statements
