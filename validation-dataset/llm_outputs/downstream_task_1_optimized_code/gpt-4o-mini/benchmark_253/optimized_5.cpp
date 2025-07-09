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
    list.reserve(10); // Reserve a reasonable amount of space
    list.push_back(path.name);
    uint64_t id = path.parentId;
    while (id) {
        FilePath p = db.get(id);
        assert(!p.name.empty());
        list.push_back(p.name); // Append to the end instead of inserting at the beginning
        id = p.parentId;
    }

    // Concatenate strings using a single reserve and build step
    std::string ret;
    ret.reserve(1 + list.size() + std::accumulate(list.begin(), list.end(), 0, [](const auto& total, const auto& str) { return total + str.size(); })); // Precalculate required size
    ret.push_back('/'); // Start with leading slash

    for (size_t i = list.size(); i > 0; --i) {
        if (i < list.size())
            ret.push_back('/'); // Add separator only between elements
        ret += list[i - 1]; // Access in reverse order for correct path
    }
    return ret;
}