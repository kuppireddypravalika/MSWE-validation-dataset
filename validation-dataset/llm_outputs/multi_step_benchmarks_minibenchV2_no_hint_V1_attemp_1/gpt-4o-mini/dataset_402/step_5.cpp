#include <vector>
#include <string>
#include <algorithm>

long long render(const std::vector<std::string>& items,
                 std::size_t scrollOffset,
                 int lineHeight,
                 int areaHeight)
{
    long long sum = 0;
    int y = -static_cast<int>(scrollOffset);
    const int itemsSize = static_cast<int>(items.size());
    int maxVisibleLines = areaHeight / lineHeight;

    // Calculate the starting and ending index based on the scroll and line height
    int startIdx = std::max(0, y / lineHeight);
    int endIdx = std::min(itemsSize, startIdx + maxVisibleLines);

    // Iterate only over the visible items
    for (int i = startIdx; i < endIdx; ++i) {
        const std::string& item = items[i];
        for (char c : item) {
            sum += c;
        }
        sum += y;
        y += lineHeight;
    }
    return sum;
}

// Explicit template instantiations (if any) go here