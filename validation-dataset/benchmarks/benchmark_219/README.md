# Benchmark 219: Faster initializer list cloning

This benchmark is derived from commit [`ce9e08b`](https://github.com/mrousavy/BrabeNetz/commit/ce9e08b22a97fbfccad7c236d08e3394c8356d51) by Marc Rousavy.
The change replaces manual copying of an `initializer_list` into a `std::vector` with
constructing a temporary vector directly from the list and passing it as an rvalue.
This avoids an extra copy when creating the network topology.

The goal is to verify that using an rvalue vector improves performance.
