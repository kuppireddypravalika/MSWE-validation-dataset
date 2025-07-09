# Benchmark 372: Moving return value to avoid copy

This benchmark is based on commit [`4dabd59`](https://github.com/PowerDNS/pdns/commit/4dabd59a849a5e464659f9e38bcc3961a099d45f) from the PowerDNS project. The patch changed `MiniCurl::getURL` to return the internal buffer using `std::move` instead of copying the string.

The original implementation created a new `std::string` from `d_data`, incurring a potentially large memory allocation and copy. The optimized version transfers ownership of `d_data` directly to the caller, clearing the internal buffer afterward. This avoids the costly copy when downloading large payloads.

The benchmark simulates repeated downloads of a sizable payload and measures the time taken to retrieve the data using the original and optimized implementations.
