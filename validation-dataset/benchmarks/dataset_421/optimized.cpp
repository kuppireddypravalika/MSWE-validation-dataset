#include "notification_area.hpp"

std::size_t NotificationArea::notificationIndex(Point pixelPosition) {
    const auto estimatedIndex = static_cast<std::size_t>((mRect.endPoint().y - pixelPosition.y) / IconPaddedHeight);
    if (estimatedIndex < mNotificationList.size() && notificationRect(estimatedIndex).contains(pixelPosition)) {
        return estimatedIndex;
    }
    return NoSelection;
}
