# Benchmark 172: FFTW plan measure vs estimate

This benchmark is based on commit [403918b](https://github.com/JeanPierreCimalando/commit/403918bc123da616863459b9de6ff5de770205e4) which changed the FFTW planning flags from `FFTW_ESTIMATE` to `FFTW_MEASURE`. The measured plan requires additional setup time but produces a more efficient FFT plan for repeated execution.

The benchmark replicates this behaviour with a simple STFT-like class that performs real to complex FFTs. The original version builds FFT plans without any precomputation, while the optimized version precomputes sine and cosine tables during plan creation. Executing many FFTs shows the runtime improvement of the measured plan.

Default size is 2048 samples with 200 iterations to ensure the original version runs for more than 10 seconds.
