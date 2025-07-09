#include <regex>
#include <string>
#include <string_view>
#include <vector>

size_t process(const std::vector<std::string>& hrefs,
               const std::string& newUrl,
               bool ignoreHidden)
{
    size_t sum = 0;
    std::regex hidden_regex("/\.""); // construct regex once outside the loop
    for (const std::string& href : hrefs) {
        // Use std::string_view to avoid copying the string
        std::string_view hrefValue = href;

        // Move the comparison directly to the check
        if (hrefValue == newUrl) {
            sum += hrefValue.size();
        }

        if (ignoreHidden && std::regex_search(hrefValue, hidden_regex)) {
            continue;
        }
    }
    return sum;
}
