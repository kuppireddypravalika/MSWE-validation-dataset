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
    std::regex hidden_regex("/\." );  // Move regex construction outside the loop

    for (const auto& href : hrefs) {
        if (ignoreHidden && std::regex_search(href, hidden_regex)) {
            continue;
        }

        // Use href directly to avoid an unnecessary copy
        if (href == newUrl) {
            sum += href.size();
        }
    }
    return sum;
}
