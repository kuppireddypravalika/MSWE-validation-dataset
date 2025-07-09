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
    for (std::size_t i = 0; i < items.size(); ++i) {
        bool visible = (y + lineHeight > 0) && (y < areaHeight);
        if (visible) {
            for (char c : items[i]) sum += c;
            sum += y;
        }
        y += lineHeight;
    }
    return sum;
}
