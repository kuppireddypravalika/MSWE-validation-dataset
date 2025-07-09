# Benchmark 226: Buchberger pair selection

This benchmark models a patch from the Giac computer algebra system ([commit a24ed88](https://github.com/geogebra/giac/commit/a24ed881e0f8bed9ed4dd52d26a6617f92ec264b)) which enabled choosing the pair with the smallest coefficient degree during Buchberger's algorithm. Selecting the pair with the lowest coefficient degree reduces later workload.

Our microbenchmark simulates this behaviour using vectors of pairs. The `original` version always processes the first candidate pair while the `optimized` version scans the candidates and selects the one with the smallest degree sum. Both implementations produce identical final states but the optimized variant avoids expensive iterations.
