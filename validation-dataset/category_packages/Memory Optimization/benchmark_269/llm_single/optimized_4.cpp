#include <regex>
#include <string>
#include <string_view>
#include <vector>

size_t process(const std::vector<std::string>& hrefs,
               const std::string& newUrl,
               bool ignoreHidden)
{
    size_t sum = 0;
    const std::regex hidden_regex("/\\.");  // Pre-compiled regex
    for (const auto& href : hrefs) {  // Use auto& to avoid copies
        if (ignoreHidden && std::regex_search(href, hidden_regex)) {
            continue;
        }
        if (href == newUrl) {
            sum += href.size();  // Use href directly
        }
    }
    return sum;
}

// Explicit template instantiation statements go here if any.
