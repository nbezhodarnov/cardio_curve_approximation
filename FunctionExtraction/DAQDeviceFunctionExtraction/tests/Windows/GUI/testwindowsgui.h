#ifndef TESTWINDOWSGUI_H
#define TESTWINDOWSGUI_H

#include <QTimer>

struct Device {
    double max;
    double min;
    float buffer[1000];
    QTimer timer;
    int i;
    QMetaObject::Connection connection;
};

extern Device device;

#endif // TESTWINDOWSGUI_H
