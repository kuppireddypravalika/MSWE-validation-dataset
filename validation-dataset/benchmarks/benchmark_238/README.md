# Benchmark 238: BitSet hashing with MurmurHash

This benchmark is based on commit [903e4f8](https://github.com/devosoft/Empirical/commit/903e4f8e92f2d2c13fe07bd413a3173dfa699bbe) from the Empirical project. The change replaced a byte‑by‑byte combination routine used in `std::hash<BitSet>` with a single call to `murmur_hash` over the underlying byte array.

The optimized version avoids per‑byte `hash_combine` calls and hashes the entire sequence in one pass.
