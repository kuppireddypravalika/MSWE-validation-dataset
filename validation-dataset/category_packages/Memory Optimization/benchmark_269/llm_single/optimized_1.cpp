#include <regex>
#include <string>
#include <string_view>
#include <vector>
#include <numeric>

size_t process(const std::vector<std::string>& hrefs,
               const std::string& newUrl,
               bool ignoreHidden)
{
    size_t sum = 0;
    std::regex hidden_regex("/\.\"); // Move regex construction outside loop

    for (const std::string& href : hrefs) {
        if (ignoreHidden && std::regex_search(href, hidden_regex)) {
            continue;
        }

        if (href == newUrl) {
            sum += href.size();
        }
    }
    return sum;
}

// Explicit template instantiation, if any exists should be placed here.
