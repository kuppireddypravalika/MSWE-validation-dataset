#include "ldap.hpp"
#include <string>
#include <vector>

// Optimized version matching commit 3142cf5: perform case-insensitive
// attribute lookup only once on the first result
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
        bool isFirstResult = true;
        std::string realAttributeName = attribute;
        std::transform(realAttributeName.begin(), realAttributeName.end(), realAttributeName.begin(), ::tolower);
        while (operation.waitForResult(id, LdapQueryTimeout) == LdapOperation::RES_SEARCH_ENTRY) {
            if (isFirstResult) {
                isFirstResult = false;
                for (const auto& kv : operation.object().attributes()) {
                    std::string keyLower = kv.first;
                    std::transform(keyLower.begin(), keyLower.end(), keyLower.begin(), ::tolower);
                    if (keyLower == realAttributeName) {
                        realAttributeName = kv.first;
                        break;
                    }
                }
            }
            for (const auto& value : operation.object().values(realAttributeName)) {
                entries.push_back(value);
            }
        }
    }
    return entries;
}
