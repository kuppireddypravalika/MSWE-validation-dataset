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
        for (const auto& atom : range) {
            if (_ordinalOverrideMap.find(atom) == _ordinalOverrideMap.end()) {
                _ordinalOverrideMap[atom] = index++;
                auto followOn = _followOnRoots.find(atom);
                if (followOn != _followOnRoots.end()) {
                    const AtomPtr nextAtom = followOn->second;
                    for (; nextAtom != nullptr; nextAtom = _followOnNexts[nextAtom]) {
                        if (_ordinalOverrideMap.find(nextAtom) == _ordinalOverrideMap.end()) {
                            _ordinalOverrideMap[nextAtom] = index++;
                        }
                    }
                }
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
        if (i + 1 < N) {
            pass._followOnNexts[&atoms[i]] = &atoms[i + 1];
        }
    }
    pass._followOnNexts[&atoms.back()] = nullptr;

    pass.buildOrdinalOverrideMap(range);

    size_t sum = 0;
    sum = pass._ordinalOverrideMap.size() ? (pass._ordinalOverrideMap.begin()->second + (pass._ordinalOverrideMap.size() - 1) * (pass._ordinalOverrideMap.size() - 1) / 2) : 0;

    return sum;
}

// Explicit template instantiation statements
