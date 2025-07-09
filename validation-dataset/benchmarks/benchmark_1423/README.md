# Benchmark 1423: OpenMP parallel kmer heap

This benchmark is derived from [commit e40b6e6](https://github.com/proywm/validation-dataset/commit/e40b6e6cef12695352948d89c21bdc6898d4d8be) which enabled OpenMP parallelisation in a heavy loop building a kmer heap. The original implementation processed all kmers sequentially while the patch added `#pragma omp parallel for` to distribute work across threads.

The simplified benchmark generates a random DNA sequence and extracts kmers of several lengths. Each kmer is compared with its reverse complement and the lexicographically smaller string is inserted into a heap. The workload mirrors the costly substring and reverse complement operations of the original code. The optimised version parallelises the inner loop using OpenMP to improve performance.
