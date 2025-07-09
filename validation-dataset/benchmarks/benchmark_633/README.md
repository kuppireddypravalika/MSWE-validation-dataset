# Benchmark 633: smart temporaries in convolution

This benchmark is derived from [etl](https://github.com/wichtounet/etl) commit `c3822e0`
which introduces the use of **smart temporaries** for 1D and 2D convolution
expressions. Before the change the convolution implementation forwarded the
input and kernel expressions directly to the convolution kernel. After the
patch both operands are first materialized when the dimensionality is 1 or 2,
reducing the overhead of repeatedly evaluating complex expressions.

The benchmark models a simple 2D convolution where both the input matrix and the
kernel are provided as lazy addition expressions. The optimized version copies
these expressions to temporary matrices before running the convolution.
