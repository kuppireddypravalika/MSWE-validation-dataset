# Benchmark 306: REP instruction optimization

This benchmark originates from commit [eb27ad6](https://github.com/proywm/benchmark-repo/commit/eb27ad6c19bbdcab70a4235d25a350bedfc338cc) which optimizes the **REP** instruction loop in a DSP emulator. The original implementation fetched the opcode's function pointer on every iteration. The patch caches this function pointer once before entering the loop, reducing dispatch overhead.

The benchmark reproduces this scenario with a simplified DSP class where `rep_exec` repeatedly executes the same opcode. The optimized version stores the member function pointer before the loop, whereas the original retrieves it each time through `execOp`.
