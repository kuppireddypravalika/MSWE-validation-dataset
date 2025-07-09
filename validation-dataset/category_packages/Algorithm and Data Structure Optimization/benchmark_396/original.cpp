#include <cstddef>

struct Position {
    bool non_pawn_material(int /*side*/) const { return true; }
};

struct Stack { int staticEval; };

__attribute__((noinline)) int null_search(int depth) {
    volatile int sum = 0;
    for (int i = 0; i < depth * 100000; ++i)
        sum += i;
    return sum;
}

int search(Position& pos, Stack* ss, int depth, int eval, int beta) {
    bool PvNode = false;
    if (!PvNode && depth >= 2 && eval >= beta && pos.non_pawn_material(0)) {
        ss->staticEval = eval; // store eval like real code
        null_search(depth);
    }
    return 1; // deterministic result regardless of pruning
}

size_t run(size_t loops) {
    Position pos;
    Stack ss{};
    size_t sum = 0;
    for (size_t i = 0; i < loops; ++i) {
        int depth = static_cast<int>(i % 10) + 1; // depths 1..10
        ss.staticEval = (i % 5 == 0) ? 5 : -5;   // rarely >= beta
        sum += search(pos, &ss, depth, 10, 0);
    }
    return sum;
}
