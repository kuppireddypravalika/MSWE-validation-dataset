#include <string>
#include <optional>
#include <stdexcept>

struct ParsedURL {
    std::string service;
    unsigned version;
    std::string profile;
    std::string query;
};

std::optional<ParsedURL> parseURL(const std::string &url) {
    if (url.length() < 6) return std::nullopt; // Early exit for short URLs
    ParsedURL out;
    size_t pos1 = url.find('/');
    if (pos1 == std::string::npos || pos1 + 1 >= url.length()) return std::nullopt;
    size_t pos2 = url.find('/v', pos1 + 1);
    if (pos2 == std::string::npos || pos2 + 2 >= url.length()) return std::nullopt;
    size_t pos3 = url.find('/', pos2 + 2);
    if (pos3 == std::string::npos || pos3 + 1 >= url.length()) return std::nullopt;
    size_t pos4 = url.find('/', pos3 + 1);
    if (pos4 == std::string::npos) pos4 = url.length();

    out.service = url.substr(pos1 + 1, pos2 - pos1 - 1);
    try {
        out.version = static_cast<unsigned>(std::stoul(url.substr(pos2 + 2, pos3 - pos2 - 2)));
    } catch (const std::invalid_argument&) { return std::nullopt; }
    out.profile = url.substr(pos3 + 1, pos4 - pos3 - 1);
    out.query = (pos4 < url.length()) ? url.substr(pos4 + 1) : "";

    return out;
}