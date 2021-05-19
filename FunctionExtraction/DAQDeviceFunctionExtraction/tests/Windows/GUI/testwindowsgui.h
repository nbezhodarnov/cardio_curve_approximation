#ifndef TESTWINDOWSGUI_H
#define TESTWINDOWSGUI_H

#include "FunctionExtraction/DAQDeviceFunctionExtraction/olmem.h"

#include <windows.h>
#include <math.h>
#include <vector>
#include <QTimer>

enum BufferStatus {
    Busy,
    ReadyToRead,
    ReadyToUpdate
};

struct buffer {
    float *buf;
    unsigned int size;
    HBUF handle;
    BufferStatus status = ReadyToUpdate;

    buffer(unsigned int);
    ~buffer();
};

struct Device {
    double max;
    double min;
    QTimer timer;
    int value;
    unsigned int buffer_index = 0;
    QMetaObject::Connection connection;
    HWND hwnd;
    long parametr;
    std::vector<buffer*> buffers;
    bool connected = false;

    ~Device();
};

extern Device device;

#endif // TESTWINDOWSGUI_H
