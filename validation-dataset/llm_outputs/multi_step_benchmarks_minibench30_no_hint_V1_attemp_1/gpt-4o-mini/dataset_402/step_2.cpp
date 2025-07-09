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
    const std::size_t itemCount = items.size();
    
    // Pre-compute variables to avoid recalculating in each iteration
    const int areaBottom = areaHeight;
    const int lineBottomBoundary = (areaBottom / lineHeight) + 1;
    
    // Limit loop to visible item indices only
    std::size_t startIndex = std::max<std::size_t>(0, static_cast<std::size_t>(y / lineHeight));
    std::size_t endIndex = std::min(itemCount, static_cast<std::size_t>(startIndex + lineBottomBoundary));
    
    for (std::size_t i = startIndex; i < endIndex; ++i) {
        int currentY = y + (i * lineHeight);
        if (currentY + lineHeight > 0) { // Only add if visible
            for (char c : items[i]) sum += c;
            sum += currentY;
        }
    }
    return sum;
}

// Explicit template instantiations (if any) would go here