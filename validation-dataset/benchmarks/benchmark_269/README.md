# Benchmark 269: Static regex for hidden file detection

This benchmark models a performance patch from KDE's `KRecentDocument` module
([commit 45b7d1c](https://invent.kde.org/frameworks/kio/-/commit/45b7d1cdb08b0de5784a804e9e10d6fded24d49c)).
The original code created a new `QRegularExpression` each time a bookmark was
processed and matched it against the bookmark's `href` attribute. The patch
changed this to reuse a static regex and operate on `QStringView` via
`matchView()`, eliminating repeated allocations and string constructions.

Our simplified reproduction iterates over many file paths, filters hidden ones
using a regular expression and sums the lengths of matching entries. The
`original` version constructs the regex and copies the string on every
iteration, while the `optimized` version uses a static `std::regex` and
`std::string_view`.

The workload is tuned so the original implementation runs for several seconds to
make the difference measurable.
