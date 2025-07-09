# Benchmark 295: array_pod_sort vs std::sort

This benchmark reproduces an optimization from LLVM commit [82c2ee6](https://github.com/llvm/llvm-project/commit/82c2ee67dcef4ed9f1d40824cebc3edc2eb9a14c) where `std::sort` was replaced with `array_pod_sort` when sorting small attribute sets. The specialized routine avoids overhead from calling a comparison function for each element.

The benchmark sorts a vector of simple `Attribute` structs many times, mimicking the original control flow in `AttributeSetNode::get`. It measures the cost of using a non-inlined comparator (`std::sort`) versus an inlined POD sort.
