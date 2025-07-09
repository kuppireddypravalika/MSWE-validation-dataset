# dataset_402: ListBox visible item rendering

This benchmark is derived from commit `83308da4` of [OutpostHD](https://github.com/ldicker83/OPHD) which optimized the `ListBox::update` method. The original implementation iterated over every item in the list and attempted to draw them even if most were outside the visible scroll area. The patch computes the first and last visible indices and only draws items that will appear on screen.

The benchmark models a simplified list box rendering loop. The original version processes all items while discarding those clipped outside the viewport. The optimized version only visits the visible items. Both versions produce the same on-screen result, but the optimized version avoids unnecessary work when the list is large.
