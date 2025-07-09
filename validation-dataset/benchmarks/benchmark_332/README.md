# Benchmark 332: TSYS01 powf optimization

This benchmark captures commit [870183f](https://github.com/ArduPilot/ardupilot/commit/870183f26bd774a0073779f14519712a5cbf50fb) from the ArduPilot project.
The change replaced calls to `std::pow` with the float specific `std::powf` when
computing the temperature in `TSYS01::_calculate`.
Using `powf` avoids unnecessary promotion to double and improves performance.

The benchmark runs the simplified temperature calculation many times to
highlight the runtime difference.
