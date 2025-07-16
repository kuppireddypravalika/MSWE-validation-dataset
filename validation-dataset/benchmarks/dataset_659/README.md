# Benchmark 2075: LDAP attribute case-insensitive lookup

This benchmark is based on [iTALC](https://github.com/ITALC/italc) commit `3142cf5` which modified
`LdapDirectory::queryAttributes` to match LDAP attribute names case-insensitively.
The optimized implementation determines the actual attribute name from the first
search result and reuses it for all remaining entries instead of performing a
case-insensitive lookup for every result.

The benchmark constructs many LDAP objects with mixed-case attribute names to
measure the benefit of resolving the real attribute name only once.
