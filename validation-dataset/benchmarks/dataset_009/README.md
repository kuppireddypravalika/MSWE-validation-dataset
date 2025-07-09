# Dataset 009: search and replace bug fix

This benchmark is based on [Aegisub](https://github.com/Aegisub/Aegisub) commit `7342c2b6171e` which corrected the case-insensitive search and replace logic in `dialog_search_replace.cpp`. The original implementation failed to append the remaining text after the last match and also did not preallocate memory for the result string. The patch preallocates the left string and concatenates the leftover part, fixing the bug and improving performance when repeatedly performing replacements.
