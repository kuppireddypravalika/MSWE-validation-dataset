# Benchmark 179: Cached Camera Device List

This benchmark replicates a performance optimization from the EGT project
([commit c294e6d](https://github.com/egt-project/egt/commit/c294e6d26a4fe0a2b5986f66397d341b3fca9f92)).
The original implementation rebuilt the camera device list on every call by
querying the device monitor. The patch simply returns the cached `m_devices`
vector maintained by the watcher thread.

The benchmark models this behavior with a simplified monitor and camera class to
measure the cost of repeatedly enumerating devices versus returning a cached
copy.
