#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include <vector>

struct Atom { int id; };

class LayoutPassOptimized {
public:
    using AtomPtr = const Atom*;
    using AtomToAtomT = std::unordered_map<AtomPtr, AtomPtr>;
    using AtomToOrdinalT = std::unordered_map<AtomPtr, std::uint64_t>;

    AtomToAtomT _followOnRoots;
    AtomToAtomT _followOnNexts;
    AtomToOrdinalT _ordinalOverrideMap;

    void buildOrdinalOverrideMap(const std::vector<AtomPtr>& range) {
        std::uint64_t index = 0;
        for (auto ai : range) {
            const Atom* atom = ai;
            auto start = _followOnRoots.find(atom);
            if (start != _followOnRoots.end()) {
                const Atom* nextAtom = start->second;
                while (nextAtom != nullptr) {
                    if (_ordinalOverrideMap.insert({ nextAtom, index }).second) {
                        index++;
                    }
                    nextAtom = _followOnNexts[nextAtom];
                }
            } else {
                _ordinalOverrideMap[atom] = index;
            }
        }
    }
};

size_t run(size_t N) {
    LayoutPassOptimized pass;
    std::vector<Atom> atoms(N);
    std::vector<const Atom*> range;
    range.reserve(N);
    for (size_t i = 0; i < N; ++i) {
        atoms[i].id = static_cast<int>(i);
        range.push_back(&atoms[i]);
        pass._followOnRoots[&atoms[i]] = &atoms[0];
        if (i + 1 < N)
            pass._followOnNexts[&atoms[i]] = &atoms[i + 1];
    }
    pass._followOnNexts[&atoms.back()] = nullptr;

    pass.buildOrdinalOverrideMap(range);

    size_t sum = 0;
    for (const auto& kv : pass._ordinalOverrideMap)
        sum += kv.second;
    return sum;
}

// Explicit template instantiation statements
