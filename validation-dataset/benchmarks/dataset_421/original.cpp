#include "notification_area.hpp"

std::size_t NotificationArea::notificationIndex(Point pixelPosition) {
    for (std::size_t count = 0; count < mNotificationList.size(); ++count) {
        if (notificationRect(count).contains(pixelPosition)) {
            return count;
        }
    }
    return NoSelection;
}
