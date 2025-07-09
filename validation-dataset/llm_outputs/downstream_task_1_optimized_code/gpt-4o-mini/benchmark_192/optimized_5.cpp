#include <string>
#include <optional>
#include <array>

struct ParsedURL {
    std::string service;
    unsigned version;
    std::string profile;
    std::string query;
};

std::optional<ParsedURL> parseURL(const std::string &url) {
    const std::array<std::string::const_iterator, 4> sections = {
        url.begin(), url.end(), url.begin(), url.end()
    };
    const size_t size = url.size();
    if (size == 0) return std::nullopt;

    auto it = sections[0];
    auto end = sections[1];

    // Check for the leading '/' character
    if (*it != '/') return std::nullopt;
    ++it;

    // Read service
    std::string service;
    while (it != end && isalpha(*it)) {
        service += *it;
        ++it;
    }
    if (service.empty()) return std::nullopt;

    // Check for '/v'
    if (it == end || *it != 'v') return std::nullopt;
    ++it;

    // Read version
    std::string versionStr;
    while (it != end && isdigit(*it)) {
        versionStr += *it;
        ++it;
    }
    if (versionStr.empty()) return std::nullopt;
    unsigned version = static_cast<unsigned>(std::stoul(versionStr));

    // Check for '/'
    if (it == end || *it != '/') return std::nullopt;
    ++it;

    // Read profile
    std::string profile;
    while (it != end && isalpha(*it)) {
        profile += *it;
        ++it;
    }
    if (profile.empty()) return std::nullopt;

    // Check for '/' and read query
    if (it == end || *it != '/') return std::nullopt;
    ++it;

    std::string query(it, end);

    // Construct and return the ParsedURL object
    ParsedURL out{std::move(service), version, std::move(profile), std::move(query)};
    return out;
}