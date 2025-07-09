#include <regex>
#include <string>
#include <vector>
#include <string_view>
#include <numeric>

size_t process(const std::vector<std::string>& hrefs,
               const std::string& newUrl,
               bool ignoreHidden)
{
    size_t sum = 0;
    std::regex hidden_regex("/\.""); // Move regex construction outside the loop

    for (const auto& href : hrefs) { // Use const auto& to avoid unnecessary copies
        bool inRightBookmark = (href == newUrl);

        if (ignoreHidden && std::regex_search(href, hidden_regex)) {
            continue;
        }

        if (inRightBookmark) {
            sum += href.size(); // Directly use href without additional copy
        }
    }
    return sum;
}

// Explicit template instantiation statements
