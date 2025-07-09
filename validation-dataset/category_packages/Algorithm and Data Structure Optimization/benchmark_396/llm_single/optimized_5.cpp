#include <cstddef>
#include <immintrin.h>

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

inline void optimized_search(Position& pos, Stack* ss, int depth, int eval, int beta) {
    if (depth >= 2 && eval >= beta && pos.non_pawn_material(0)) {
        ss->staticEval = eval; // store eval like real code
        null_search(depth);
    }
}

size_t run(size_t loops) {
    Position pos;
    Stack ss{};
    size_t sum = 0;
    for (size_t i = 0; i < loops; ++i) {
        int depth = (int)(i % 10) + 1; // depths 1..10 
        ss.staticEval = (i % 5 == 0) ? 5 : -5; // rarely >= beta
        optimized_search(pos, &ss, depth, 10, 0);
        sum += 1; // deterministic result regardless of pruning
    }
    return sum;
}

// Explicit template instantiation statements (if any) go here.
