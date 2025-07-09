# Dataset 421: NotificationArea index optimization

Derived from Open-PHX "OPHD" commit `4e67329c0ad4a560fc5c956f3c0ae805a347d378`.
The patch optimizes `NotificationArea::notificationIndex` by eliminating a loop
over all rectangles. Instead it computes an estimated index from the y
coordinate and checks only that rectangle. The change avoids scanning the entire
notification list when determining which icon was clicked.
