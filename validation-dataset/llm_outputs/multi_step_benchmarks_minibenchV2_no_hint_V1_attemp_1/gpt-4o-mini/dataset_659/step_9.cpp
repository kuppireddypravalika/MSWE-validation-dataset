#include "ldap.hpp"
#include <string>
#include <vector>
#include <unordered_set>

std::vector<std::string> queryAttributes(
    LdapOperation& operation,
    const std::string& dn,
    const std::string& attribute,
    const std::string& filter = "(objectclass=*)",
    int scope = 0) {
    std::vector<std::string> entries;
    const int LdapQueryTimeout = 3000;
    int id = operation.search(dn, scope, filter, {attribute});
    if (id != -1) {
        std::unordered_set<std::string> seen_values;
        while (operation.waitForResult(id, LdapQueryTimeout) == LdapOperation::RES_SEARCH_ENTRY) {
            const auto& values = operation.object().values(attribute);
            for (const auto& value : values) {
                // Store unique values only
                if (seen_values.insert(value).second) {
                    entries.push_back(value);
                }
            }
        }
    }
    return entries;
}

// Explicit template instantiations (unchanged) 