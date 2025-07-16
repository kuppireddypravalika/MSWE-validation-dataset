#include "ldap.hpp"
#include <string>
#include <vector>

// Simplified version of LdapDirectoryPrivate::queryAttributes before the patch
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
        while (operation.waitForResult(id, LdapQueryTimeout) == LdapOperation::RES_SEARCH_ENTRY) {
            for (const auto& value : operation.object().values(attribute)) {
                entries.push_back(value);
            }
        }
    }
    return entries;
}
