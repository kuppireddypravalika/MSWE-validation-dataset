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

    // Calculate start and end indices for visible items
    int startIdx = std::max(0, y / lineHeight);
    int endIdx = std::min(itemsSize, startIdx + maxVisibleLines);

    // Iterate only over visible items
    for (int i = startIdx; i < endIdx; ++i) {
        const std::string& item = items[i];
        // Add line index contribution
        sum += y; 
        for (char c : item) {
            sum += c;
        }
        // Update y-coord for next item
        y += lineHeight;
    }
    return sum;
}

// Explicit template instantiations (if any) go here