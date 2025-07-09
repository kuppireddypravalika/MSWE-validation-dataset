#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

struct FilePath { uint64_t parentId; std::string name; };

struct IdFilenameDB {
    std::vector<FilePath> records;
    FilePath get(uint64_t id) const { return records[id]; }
};

std::string get(const IdFilenameDB& db, uint64_t docId);

IdFilenameDB createDB(size_t depth)
{
    IdFilenameDB db;
    db.records.resize(depth + 1);
    for (size_t i = 1; i <= depth; ++i) {
        db.records[i].parentId = i - 1;
        db.records[i].name = "node" + std::to_string(i);
    }
    return db;
}

int main(int argc, char* argv[])
{
    std::string mode = "perf";
    size_t depth = 100;
    int repeat = 310000; // workload chosen so original runs ~5s

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--depth=",0)==0) depth = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    IdFilenameDB db = createDB(depth);

    if (mode == "correct") {
        std::string out = get(db, depth);
        std::cout << out << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;

    auto t1 = clock::now();
    for (int i = 0; i < repeat; ++i)
        get(db, depth);
    auto t2 = clock::now();

    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";

    return 0;
}
