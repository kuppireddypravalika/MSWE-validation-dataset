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
        return std::string();

    std::string ret = path.name;
    uint64_t id = path.parentId;
    while (id) {
        FilePath p = db.get(id);
        assert(!p.name.empty());
        ret = p.name + '/' + ret;
        id = p.parentId;
    }

    ret.insert(ret.begin(), '/');
    return ret;
}
