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
        return "";

    std::vector<std::string> list;
    list.reserve(10); // Reserve initial space to avoid multiple reallocations
    list.push_back(path.name);
    uint64_t id = path.parentId;
    while (id) {
        FilePath p = db.get(id);
        assert(!p.name.empty());
        list.push_back(p.name); // Use push_back for efficiency
        id = p.parentId;
    }

    std::string ret;
    ret.reserve(list.size() * 2); // Estimate space for the final string
    ret.push_back('/'); // Start with the leading slash
    for (size_t i = list.size(); i > 0; --i) { // Iterate in reverse
        if (i < list.size())
            ret.append("/"); // Add the slash only between names
        ret.append(list[i - 1]); // Append the name
    }
    return ret;
}