#include <regex>
#include <string>
#include <string_view>
#include <vector>

size_t process(const std::vector<std::string>& hrefs,
               const std::string& newUrl,
               bool ignoreHidden)
{
    size_t sum = 0;
    for (const std::string& href : hrefs) {
        // Using auto here results in a copy of the string each iteration
        auto hrefValue = href; // copy like QString -> QString
        bool inRightBookmark = hrefValue == newUrl;

        if (ignoreHidden) {
            // Regex constructed on every iteration
            std::regex hidden_regex("/\\.");
            if (std::regex_search(hrefValue, hidden_regex)) {
                continue;
            }
        }

        if (inRightBookmark) {
            sum += hrefValue.size();
        }
    }
    return sum;
}
