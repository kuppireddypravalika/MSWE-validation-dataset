#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include <vector>
#include <numeric>

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
        for (auto atom : range) {
            if (_ordinalOverrideMap.find(atom) == _ordinalOverrideMap.end()) {
                _ordinalOverrideMap[atom] = index++;
            }
            auto followIt = _followOnRoots.find(atom);
            if (followIt != _followOnRoots.end()) {
                for (const Atom* nextAtom = followIt->second; nextAtom != nullptr; nextAtom = _followOnNexts[nextAtom]) {
                    if (_ordinalOverrideMap.insert({nextAtom, index}).second) {
                        index++;
                    }
                }
            }
        }
    }
};

size_t run(size_t N) {
    LayoutPassOptimized pass;
    std::vector<Atom> atoms(N);
    std::vector<const Atom*> range(N);
    for (size_t i = 0; i < N; ++i) {
        atoms[i].id = static_cast<int>(i);
        range[i] = &atoms[i];
        pass._followOnRoots[&atoms[i]] = &atoms[0];
        if (i + 1 < N)
            pass._followOnNexts[&atoms[i]] = &atoms[i + 1];
    }
    pass._followOnNexts[&atoms.back()] = nullptr;

    pass.buildOrdinalOverrideMap(range);

    return std::accumulate(pass._ordinalOverrideMap.begin(), pass._ordinalOverrideMap.end(), size_t(0), [](size_t acc, const auto& kv) { return acc + kv.second; });
}

// Explicit template instantiation statements (if any) go here.