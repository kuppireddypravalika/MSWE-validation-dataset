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
        return "/";  // return root in case of no name

    std::vector<std::string> list;
    list.reserve(32);  // Reserve default size for efficiency
    uint64_t id = path.parentId;

    // Insert the current file path name
    list.push_back(path.name);

    // Prepend names while traversing parent paths
    while (id) {
        FilePath p = db.get(id);
        assert(!p.name.empty());
        list.push_back(p.name);
        id = p.parentId;
    }

    // Build the final string
    std::string ret;
    ret.reserve(1 + list.size() * 10); // Arbitrary size for efficiency (adjust if needed)
    ret.push_back('/');  // Start with root
    for (auto it = list.rbegin(); it != list.rend(); ++it) {
        if (it != list.rbegin()) // Append slash before all but the first element
            ret.push_back('/');
        ret += *it;
    }
    return ret;
}
