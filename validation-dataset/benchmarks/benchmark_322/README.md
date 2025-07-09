# Benchmark 322: Avoid double name normalization in TClass::GetClass

This benchmark derives from ROOT commit [6631b55](https://github.com/root-project/root/commit/6631b55852885cfcc8b511900df23f3bee747ee3). The patch avoids calling `TROOT::FindSTLClass` after already computing the normalized name inside `TClass::GetClass`. Instead, the code directly calls `gInterpreter->GenerateTClass` with the normalized name.

The original implementation normalized the container name twice which incurred extra work when creating a `TClass` for STL containers.
