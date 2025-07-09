#pragma once
#include <vector>
#include <cstddef>

struct Point { int x; int y; };

struct Rect {
    int x, y, w, h;
    bool contains(Point p) const {
        return p.x >= x && p.x < x + w && p.y >= y && p.y < y + h;
    }
    Point endPoint() const { return {x + w, y + h}; }
};

struct NotificationArea {
    static constexpr int IconWidth = 32;
    static constexpr int IconHeight = 32;
    static constexpr int IconPaddingX = 8;
    static constexpr int IconPaddingY = 2;
    static constexpr int IconPaddedWidth = IconWidth + IconPaddingX * 2;
    static constexpr int IconPaddedHeight = IconHeight + IconPaddingY * 2;
    static constexpr std::size_t NoSelection = static_cast<std::size_t>(-1);

    std::vector<int> mNotificationList;
    Rect mRect;

    explicit NotificationArea(std::size_t count = 0)
        : mNotificationList(count),
          mRect{0, 0, IconPaddedWidth, static_cast<int>(count) * IconPaddedHeight} {}

    Rect notificationRect(std::size_t index) const {
        int rectX = mRect.x + IconPaddingX;
        int rectY = mRect.y + mRect.h - IconPaddedHeight * static_cast<int>(index + 1);
        return {rectX, rectY, IconWidth, IconHeight};
    }

    std::size_t notificationIndex(Point pixelPosition);
};
