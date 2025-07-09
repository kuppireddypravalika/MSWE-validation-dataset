# Benchmark 049: Burned warehouse job skipping

Derived from Settlers II - Return to the Roots commit `6c79a0a0` which optimized
`BurnedWarehouse::HandleEvent` by skipping loop iterations when no workers of a
job type are present. The original code still executed the random direction
logic even when `count` was zero. The patched version checks `if(count == 0)`
and continues, avoiding unnecessary random calls and loop overhead.
