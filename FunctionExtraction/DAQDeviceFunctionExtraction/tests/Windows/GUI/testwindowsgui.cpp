#include <iostream>

#include "FunctionExtraction/DAQDeviceFunctionExtraction/olmem.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/oldaapi.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/tests/Windows/GUI/testwindowsgui.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdeviceinterface.h"

buffer::buffer(unsigned int alloc_size) {
    buf = new float[alloc_size];
    size = alloc_size;
    handle = new HBUF__();
}

buffer::~buffer() {
    delete[] buf;
    delete handle;
}

Device::~Device() {
    for (unsigned int i = 0; i < buffers.size(); i++) {
        delete buffers[i];
    }
}

Device device;

ECODE WINAPI olDaEnumBoards (DABRDPROC pfnDaBrdProc, LPARAM lParam) {
    char str[11] = {68, 84, 57, 56, 48, 50, 40, 48, 48, 41, 0}, str2[7] = {68, 116, 57, 56, 48, 48, 0};
    pfnDaBrdProc((wchar_t*)str, (wchar_t*)str2, lParam);
    str[5] = 51;
    str2[5] = 51;
    pfnDaBrdProc((wchar_t*)str, (wchar_t*)str2, lParam);
    return OLNOERROR;
}

ECODE WINAPI olDaInitialize (PTSTR pszName, PHDEV) {
    std::cout << (char*)pszName << '\n';
    if (device.connected == false) {
        device.connected = true;
        return OLNOERROR;
    }
    return OLNOBOARDSINSTALLED;
}

ECODE WINAPI olDaEnumSSCaps (HDASS, UINT uiEnumCap, CAPSPROC pfnCapsProc, LPARAM lParam) {
    switch (uiEnumCap) {
    case OL_ENUM_FILTERS: {
        pfnCapsProc(0, 0.0, 0, lParam);
        break;
    }
    case OL_ENUM_RANGES: {
        pfnCapsProc(0, 10.0, -10.0, lParam);
        pfnCapsProc(0, 10.0, 0.0, lParam);
        break;
    }
    case OL_ENUM_GAINS: {
        pfnCapsProc(0, 1.0, 0, lParam);
        pfnCapsProc(0, 2.0, 0, lParam);
        pfnCapsProc(0, 4.0, 0, lParam);
        pfnCapsProc(0, 8.0, 0, lParam);
        break;
    }
    default:
        break;
    }
    return OLNOERROR;
}

ECODE WINAPI olDaGetSSCaps (HDASS, OLSSC OlSSc, PUINT puiCap) {
    switch (OlSSc) {
    case OLSSC_NUMCHANNELS: {
        *puiCap = 3;
        break;
    }
    case OLSSC_SUP_TRIGSCAN: {
        *puiCap = 1;
        break;
    }
    case OLSSC_NUMDMACHANS: {
        *puiCap = 1;
        break;
    }
    case OLSSC_RETURNS_FLOATS: {
        *puiCap = 1;
        break;
    }
    default:
        break;
    }
    return OLNOERROR;
}

ECODE WINAPI olDaGetSSCapsEx (HDASS, OLSSC OlSSc, PDBL pdCap) {
    switch (OlSSc) {
    case OLSSCE_MAXTHROUGHPUT: {
        *pdCap = 1000.0;
        break;
    }
    default:
        break;
    }
    return OLNOERROR;
}

ECODE WINAPI olDaGetDevCaps (HDEV, OLDC, PUINT) {
    return OLNOERROR;
}

ECODE WINAPI olDaGetDASS (HDEV, OLSS, UINT, PHDASS) {
    return OLNOERROR;
}

ECODE WINAPI olDaGetEncoding (HDASS, PUINT puiEnc) {
    *puiEnc = OL_ENC_BINARY;
    return OLNOERROR;
}

ECODE WINAPI olDaGetTrigger (HDASS, PUINT puiTrig) {
    *puiTrig = OL_CLOCK_RISING;
    return OLNOERROR;
}

ECODE WINAPI olDaGetClockSource (HDASS, PUINT puiClk) {
    *puiClk = OL_CLOCK_RISING;
    return OLNOERROR;
}

ECODE WINAPI olDaSetWndHandle (HDASS, HWND hWnd, LPARAM lParam) {
    device.hwnd = hWnd;
    device.parametr = lParam;
    return OLNOERROR;
}

ECODE WINAPI olDaSetEncoding (HDASS, UINT) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetClockSource (HDASS, UINT) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetTrigger (HDASS, UINT) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetChannelType (HDASS, UINT) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetDataFlow(HDASS, UINT) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetWrapMode (HDASS, UINT) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetClockFrequency (HDASS, DBL) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetDmaUsage (HDASS, UINT) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetRetriggerFrequency (HDASS, DBL) {
    return OLNOERROR;
}

ECODE WINAPI olDaConfig (HDASS) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetChannelListSize (HDASS, UINT) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetChannelListEntry (HDASS, UINT, UINT) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetGainListEntry (HDASS, UINT, DBL) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetChannelFilter (HDASS, UINT, DBL) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetRange (HDASS, DBL dMaxVoltage, DBL dMinVoltage) {
    device.max = dMaxVoltage;
    device.min = dMinVoltage;
    return OLNOERROR;
}

ECODE WINAPI olDmFreeBuffer (HBUF) {
    return OLNOERROR;
}

ECODE WINAPI olDaGetResolution (HDASS, PUINT puiBits) {
    *puiBits = 32;
    return OLNOERROR;
}

ECODE WINAPI olDaGetChannelListSize (HDASS, PUINT) {
    return OLNOERROR;
}

ECODE WINAPI olDmCallocBuffer (UINT, UINT, DWORD buffer_size, UINT, LPHBUF hBuf) {
    device.buffers.push_back(new buffer(buffer_size));
    static int y = 1;
    device.buffers[device.buffers.size() - 1]->handle->unused = y;
    y++;
    *hBuf = device.buffers[device.buffers.size() - 1]->handle;
    return OLNOERROR;
}

ECODE WINAPI olDaPutBuffer (HDASS, HBUF hBuf) {
    for (unsigned int i = 0; i < device.buffers.size(); i++) {
        if (hBuf == device.buffers[i]->handle) {
            device.buffers[i]->status = ReadyToUpdate;
            break;
        }
    }
    return OLNOERROR;
}

ECODE WINAPI olDmGetValidSamples (HBUF hBuf, DWORD* hPtr) {
    for (unsigned int i = 0; i < device.buffers.size(); i++) {
        if (hBuf == device.buffers[i]->handle) {
            if (device.buffers[i]->status == ReadyToRead) {
                *hPtr = device.buffers[i]->size;
                return OLNOERROR;
            } else {
                *hPtr = 0;
                return OLBUFFERLOCKED;
            }
            break;
        }
    }
    return OLBUFFERLOCKED;
}

ECODE WINAPI olDmGetBufferPtr (HBUF hBuf, LPVOID* hPtr) {
    for (unsigned int i = 0; i < device.buffers.size(); i++) {
        if (hBuf == device.buffers[i]->handle) {
            if (device.buffers[i]->status == ReadyToRead) {
                *hPtr = device.buffers[i]->buf;
                return OLNOERROR;
            } else {
                *hPtr = nullptr;
                return OLBUFFERLOCKED;
            }
            break;
        }
    }
    return OLBUFFERLOCKED;
}

void UpdateBuffer() {
    for (unsigned int i = 0; i < device.buffers.size(); i++) {
        if (device.buffers[i]->status == ReadyToUpdate) {
            device.buffers[i]->status = Busy;
            for (unsigned int j = 0; j < device.buffers[i]->size; j++) {
                device.buffers[i]->buf[j] = device.value;
                device.value = ((device.value + 1) % (10000 * (int)round((device.max - device.min) / 10.0)));
            }
            device.buffers[i]->status = ReadyToRead;
        }
    }
    extractData(OLDA_WM_BUFFER_DONE, 0, device.parametr);
}

ECODE WINAPI olDaStart (HDASS) {
    for (unsigned int i = 0; i < device.buffers.size(); i++) {
        device.buffers[i]->status = ReadyToUpdate;
    }
    device.timer.setInterval(100);
    device.value = 0;
    device.connection = QObject::connect(&device.timer, &QTimer::timeout, UpdateBuffer);
    device.timer.start();
    return OLNOERROR;
}

ECODE WINAPI olDaAbort (HDASS) {
    QObject::disconnect(device.connection);
    device.timer.stop();
    return OLNOERROR;
}

ECODE WINAPI olDaStop (HDASS) {
    QObject::disconnect(device.connection);
    device.timer.stop();
    return OLNOERROR;
}

ECODE WINAPI olDaFlushBuffers (HDASS) {
    return OLNOERROR;
}

ECODE WINAPI olDaGetBuffer (HDASS, PHBUF phBuf) {
    if (!device.buffers.empty() && device.buffers[device.buffer_index]->status == ReadyToRead) {
        *phBuf = device.buffers[device.buffer_index]->handle;
        device.buffer_index = (device.buffer_index + 1) % device.buffers.size();
        return OLNOERROR;
    }
    *phBuf = nullptr;
    return OLBUFFERLOCKED;
}

ECODE WINAPI olDaReleaseDASS (HDASS) {
    device.connected = false;
    return OLNOERROR;
}

ECODE WINAPI olDaTerminate (HDEV) {
    device.connected = false;
    return OLNOERROR;
}
