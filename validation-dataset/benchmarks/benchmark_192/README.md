# Benchmark 192: Reusing URL parser object

This benchmark is derived from the OSRM commit [221f70a](https://github.com/Project-OSRM/osrm-backend/commit/221f70ac7b6acf970c6882daf004d14467e4a09d) which changed the request URL parser to be a `static` variable. Constructing the parser from the Boost.Spirit DSL on every call incurred unnecessary overhead.

Our microbenchmark uses a simplified regular expression parser to model this behavior. The `original` implementation compiles the `std::regex` each time `parseURL` is called, while the `optimized` version stores the compiled regex in a static variable so it is created only once.

The workload repeatedly parses the same request string to highlight the difference in parser construction cost.

