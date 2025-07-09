#include <regex>
#include <string>
#include <string_view>
#include <vector>

size_t process(const std::vector<std::string>& hrefs,
               const std::string& newUrl,
               bool ignoreHidden)
{
    size_t sum = 0;
    std::regex hidden_regex("/\.""); // Move regex construction outside the loop

    for (const std::string& href : hrefs) {
        // No need for auto copy; we can use href directly
        bool inRightBookmark = (href == newUrl);

        if (ignoreHidden && std::regex_search(href, hidden_regex)) {
            continue;
        }

        if (inRightBookmark) {
            sum += href.size();
        }
    }
    return sum;
}

// Explicit template instantiations, if any, should be added here.