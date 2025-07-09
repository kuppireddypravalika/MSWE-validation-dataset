#include <vector>
#include <string>
#include <algorithm>

long long render(const std::vector<std::string>& items,
                 std::size_t scrollOffset,
                 int lineHeight,
                 int areaHeight)
{
    long long sum = 0;
    const int yStart = -static_cast<int>(scrollOffset);
    const std::size_t itemCount = items.size();
    const int yLimit = areaHeight;

    // Precompute visible range
    int startIndex = std::max(0, yStart / lineHeight);
    int endIndex = std::min(static_cast<int>(itemCount), (yLimit + lineHeight - 1) / lineHeight);
    
    // Iterate only through visible items
    for (int i = startIndex; i < endIndex; ++i) {
        int currentY = yStart + (i * lineHeight);
        // Ensure the item is still visible
        if (currentY >= -lineHeight && currentY < yLimit) {
            for (char c : items[i]) sum += c;
            sum += currentY;
        }
    }
    return sum;
}


// Explicit template instantiations (if any) would go here