# Benchmark 151: Using `std::comp_ellint_1` instead of Boost

This benchmark is based on the commit [3e7fc71](https://github.com/mrheld/dg/commit/3e7fc71757cb88362b940c370c0067d3277f496e) which replaced the Boost implementation of the complete elliptic integral of the first kind with the C++17 `std::comp_ellint_1` function.

The original code repeatedly evaluated `boost::math::ellint_1` during an iterative algorithm. Switching to the standard library version reduces overhead from the heavy Boost implementation.
