#include <regex>
#include <string>
#include <string_view>
#include <vector>

size_t process(const std::vector<std::string>& hrefs,
               const std::string& newUrl,
               bool ignoreHidden)
{
    size_t sum = 0;
    // Reuse regex across iterations and operate on string_view
    static const std::regex hidden_regex("/\\.");
    for (const std::string& href : hrefs) {
        std::string_view hrefValue = href; // avoid copy
        bool inRightBookmark = hrefValue == newUrl;

        if (ignoreHidden) {
            if (std::regex_search(hrefValue.begin(), hrefValue.end(), hidden_regex)) {
                continue;
            }
        }

        if (inRightBookmark) {
            sum += hrefValue.size();
        }
    }
    return sum;
}
