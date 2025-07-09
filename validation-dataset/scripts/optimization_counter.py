#!/usr/bin/env python3
"""Parse compiler logs to count explicit optimization transformations."""

import re
from typing import Dict


# Patterns keyed by metric name for GCC -fopt-info-all output
GCC_PATTERNS = {
    "loops_vectorized": r"loop vectorized",
    "loops_unrolled": r"unrolled loop",
    "loops_peeled": r"peeled loop",
    "functions_inlined": r"inlining|inlined",
    "functions_cloned": r"function cloned",
    "functions_eliminated": r"function eliminated",
    "constants_propagated": r"constant propagated|constant propagation",
    "branches_simplified": r"simplified conditional|simplified control flow|branch.*simplified",
    "common_subexpr_eliminated": r"common subexpression eliminated",
    "redundant_load_store_removed": r"redundant (load|store) eliminated",
    "instructions_eliminated": r"dead code eliminated|eliminated dead code|instructions eliminated",
    "dead_stores_removed": r"dead store eliminated",
    "branches_removed": r"branch removed|eliminating branch",
    "strength_reduced": r"strength reduced|strength reduction",
    "memory_accesses_optimized": r"memory access.*optimized|alias analysis results in a better memory access",
    "tail_calls_optimized": r"tail call optimized",
}

# Patterns for LLVM `-stats` output
LLVM_PATTERNS = {
    "loops_vectorized": r"NumVectorizedLoops",
    "loops_unrolled": r"NumUnrolledLoops",
    "loops_peeled": r"NumPeeled",
    "functions_inlined": r"NumInlined",
    "functions_cloned": r"NumCloned",
    "functions_eliminated": r"NumEliminated",
    "constants_propagated": r"NumConstantsPropagated",
    "branches_simplified": r"NumCondBranches",
    "common_subexpr_eliminated": r"NumCSE",
    "redundant_load_store_removed": r"NumRedundant",
    "instructions_eliminated": r"NumInstructionsDeleted",
    "dead_stores_removed": r"NumDeadStores",
    "branches_removed": r"NumSimplifiedCFG",
    "strength_reduced": r"NumStrengthReduced",
    "memory_accesses_optimized": r"NumMemoryOpts",
    "tail_calls_optimized": r"NumTailCalls",
}


def _count_matches(pattern: str, text: str) -> int:
    return len(re.findall(pattern, text, flags=re.IGNORECASE))


def count_from_gcc_log(log_text: str) -> Dict[str, int]:
    """Return transformation counts extracted from GCC optimization remarks."""
    return {name: _count_matches(pat, log_text) for name, pat in GCC_PATTERNS.items()}


def count_from_llvm_stats(stats_text: str) -> Dict[str, int]:
    """Return transformation counts extracted from LLVM -stats output."""
    counts: Dict[str, int] = {}
    for name, pat in LLVM_PATTERNS.items():
        match = re.search(rf"{pat}\s*=\s*(\d+)", stats_text)
        if match:
            counts[name] = int(match.group(1))
        else:
            counts[name] = 0
    return counts


if __name__ == "__main__":
    import argparse
    import json
    parser = argparse.ArgumentParser(description="Parse optimization logs")
    parser.add_argument("log", help="Log file to parse")
    parser.add_argument("--llvm", action="store_true", help="Parse LLVM -stats output")
    args = parser.parse_args()
    with open(args.log, "r", encoding="utf-8") as f:
        text = f.read()
    if args.llvm:
        result = count_from_llvm_stats(text)
    else:
        result = count_from_gcc_log(text)
    print(json.dumps(result, indent=2))
