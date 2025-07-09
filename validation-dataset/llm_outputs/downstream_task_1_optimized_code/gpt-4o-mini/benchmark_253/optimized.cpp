#include <string>
#include <vector>
#include <cassert>
#include <cstdint>

struct FilePath { uint64_t parentId; std::string name; };

struct IdFilenameDB {
    std::vector<FilePath> records;
    FilePath get(uint64_t id) const { return records[id]; }
};

std::string get(const IdFilenameDB& db, uint64_t docId)
{
    FilePath path = db.get(docId);
    if (path.name.empty())
        return "/";

    // Reserve space in the list to avoid reallocation
    std::vector<std::string> list;
    list.reserve(10); // Assume a reasonable maximum depth (10)
    list.push_back(path.name);
    uint64_t id = path.parentId;

    while (id) {
        FilePath p = db.get(id);
        assert(!p.name.empty());
        list.push_back(p.name);
        id = p.parentId;
    }

    // Build the final string with a single allocation
    size_t totalLength = 1; // Initial '/' character
    for (const auto& name : list) {
        totalLength += name.length() + 1; // for '/' or null terminator
    }
    std::string ret;
    ret.reserve(totalLength);
    ret.push_back('/'); // Start with initial '/' character

    for (auto it = list.rbegin(); it != list.rend(); ++it) {
        if (it != list.rbegin())
            ret.push_back('/');
        ret += *it;
    }

    return ret;
}

// Explicit template instantiation statements
