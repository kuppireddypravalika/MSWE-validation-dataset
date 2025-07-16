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

    // Determine the start and end indices for our visible items.
    int startIdx = std::max(0, y / lineHeight);
    int endIdx = std::min(itemsSize, startIdx + maxVisibleLines);

    for (int i = startIdx; i < endIdx; ++i) {
        for (char c : items[i]) sum += c;
        sum += y;
        y += lineHeight;
    }
    return sum;
}