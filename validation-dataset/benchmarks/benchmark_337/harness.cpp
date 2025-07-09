#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Object {
    std::string name;
    std::vector<Object*> kids;
    virtual ~Object() = default;
    void addChild(Object* c) { kids.push_back(c); }
    const std::vector<Object*>& children() const { return kids; }
    const std::string& objectName() const { return name; }
};

struct Widget : Object {
    using Object::Object;
};

template<typename T>
T* findDirectChild(Object* obj, const std::string& name);

// Build a tree with N Widget children attached to root
static void buildTree(Object& root, size_t n,
                      std::vector<std::unique_ptr<Widget>>& storage,
                      std::vector<std::string>& names)
{
    storage.reserve(n);
    names.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        names.push_back("child" + std::to_string(i));
        storage.emplace_back(new Widget{});
        storage.back()->name = names.back();
        root.addChild(storage.back().get());
    }
}

// Run the benchmark and return a deterministic sum
static size_t run(Object& root, const std::vector<std::string>& names, int repeat)
{
    size_t sum = 0;
    for (int r = 0; r < repeat; ++r) {
        for (size_t i = 0; i < names.size(); ++i) {
            Widget* w = findDirectChild<Widget>(&root, names[i]);
            if (w)
                sum += i + 1; // accumulate index to prevent optimisation
        }
    }
    return sum;
}

int main(int argc, char* argv[])
{
    std::string mode = "perf";
    size_t children = 256;
    int repeat = 18000; // scaled so the original runtime exceeds 5 seconds

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--children=", 0) == 0) children = std::stoul(arg.substr(11));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    Object root;
    std::vector<std::unique_ptr<Widget>> storage;
    std::vector<std::string> names;
    buildTree(root, children, storage, names);

    if (mode == "correct") {
        size_t result = run(root, names, 1);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        size_t result = run(root, names, repeat);
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
        // also output result to avoid optimisation
        (void)result;
    }
    return 0;
}
