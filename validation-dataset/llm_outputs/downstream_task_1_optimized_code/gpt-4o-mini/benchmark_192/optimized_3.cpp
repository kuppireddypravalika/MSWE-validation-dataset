#include <string>
#include <optional>
#include <sstream>

struct ParsedURL {
    std::string service;
    unsigned version;
    std::string profile;
    std::string query;
};

std::optional<ParsedURL> parseURL(const std::string &url) {
    std::size_t pos = 0;
    auto next_segment = [&]() {
        std::size_t next_pos = url.find('/', pos);
        std::string segment = url.substr(pos, next_pos - pos);
        pos = (next_pos == std::string::npos) ? next_pos : next_pos + 1;
        return segment;
    };
    
    std::string service = next_segment();
    if (service.empty()) return std::nullopt;
    
    std::string version_str = next_segment();
    unsigned version = 0;
    try {
        version = static_cast<unsigned>(std::stoul(version_str));
    } catch (...) {
        return std::nullopt;
    }
    
    std::string profile = next_segment();
    if (profile.empty()) return std::nullopt;
    
    std::string query = next_segment();
    
    if (url.find('/', pos) != std::string::npos) return std::nullopt; // ensure no extra segments
    
    ParsedURL out{service, version, profile, query};
    return out;
}