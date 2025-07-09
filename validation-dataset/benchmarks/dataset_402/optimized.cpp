#include <vector>
#include <string>
#include <algorithm>

long long render(const std::vector<std::string>& items,
                 std::size_t scrollOffset,
                 int lineHeight,
                 int areaHeight)
{
    const std::size_t firstVisibleIndex = scrollOffset / lineHeight;
    const std::size_t lastVisibleIndex = (scrollOffset + areaHeight + (lineHeight - 1)) / lineHeight;
    const std::size_t endVisibleIndex = std::min(lastVisibleIndex, items.size());

    long long sum = 0;
    int y = -static_cast<int>(scrollOffset % lineHeight);
    for (std::size_t i = firstVisibleIndex; i < endVisibleIndex; ++i) {
        for (char c : items[i]) sum += c;
        sum += y;
        y += lineHeight;
    }
    return sum;
}
