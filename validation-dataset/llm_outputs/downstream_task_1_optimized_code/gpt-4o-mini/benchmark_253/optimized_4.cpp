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
    const FilePath& path = db.get(docId);
    if (path.name.empty())
        return "";

    std::vector<std::string> list;
    list.reserve(10); // Reserve memory to avoid multiple reallocations
    list.push_back(path.name);
    uint64_t id = path.parentId;
    while (id) {
        const FilePath& p = db.get(id);
        assert(!p.name.empty());
        list.push_back(p.name);
        id = p.parentId;
    }

    std::string ret;
    ret.reserve(list.size() * 10); // Reserve enough memory based on expected size
    for (size_t i = list.size(); i > 0; --i) {
        ret.append(list[i - 1]);
        if (i > 1)
            ret.push_back('/');
    }
    ret.insert(ret.begin(), '/');
    return ret;
}
