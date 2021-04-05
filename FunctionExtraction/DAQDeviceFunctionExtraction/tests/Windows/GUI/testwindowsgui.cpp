#include <iostream>

#include "FunctionExtraction/DAQDeviceFunctionExtraction/olmem.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/oldaapi.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/tests/Windows/GUI/testwindowsgui.h"

Device device;

ECODE WINAPI olDaEnumBoards (DABRDPROC pfnDaBrdProc, LPARAM lParam) {
    char str[11] = {68, 84, 57, 56, 48, 50, 40, 48, 48, 41, 0}, str2[7] = {68, 116, 57, 56, 48, 48, 0};
        pfnDaBrdProc((wchar_t*)str, (wchar_t*)str2, lParam);
        str[5] = 51;
        str2[5] = 51;
        pfnDaBrdProc((wchar_t*)str, (wchar_t*)str2, lParam);
        return OLNOERROR;
}

ECODE WINAPI olDaInitialize (PTSTR pszName, PHDEV phDev) {
    std::cout << (char*)pszName << '\n';
    return OLNOERROR;
}

ECODE WINAPI olDaEnumSSCaps (HDASS hDass, UINT uiEnumCap, CAPSPROC pfnCapsProc, LPARAM lParam) {
    switch (uiEnumCap) {
    case OL_ENUM_FILTERS: {
        pfnCapsProc(0, 5.0, 0, lParam);
        pfnCapsProc(0, 10.0, 0, lParam);
        pfnCapsProc(0, 15.0, 0, lParam);
        pfnCapsProc(0, 20.0, 0, lParam);
        break;
    }
    case OL_ENUM_RANGES: {
        pfnCapsProc(0, 1.0, -1.0, lParam);
        pfnCapsProc(0, 2.0, -2.0, lParam);
        pfnCapsProc(0, 3.0, -3.0, lParam);
        pfnCapsProc(0, 4.0, -4.0, lParam);
        break;
    }
    case OL_ENUM_GAINS: {
        pfnCapsProc(0, 10.0, 0, lParam);
        pfnCapsProc(0, 20.0, 0, lParam);
        pfnCapsProc(0, 30.0, 0, lParam);
        pfnCapsProc(0, 40.0, 0, lParam);
        break;
    }
    default:
        break;
    }
    return OLNOERROR;
}

ECODE WINAPI olDaGetSSCaps (HDASS hDass, OLSSC OlSSc, PUINT puiCap) {
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

ECODE WINAPI olDaGetSSCapsEx (HDASS hDass, OLSSC OlSSc, PDBL pdCap) {
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

ECODE WINAPI olDaGetDevCaps (HDEV hDev, OLDC OlDc, PUINT puiCap) {
    return OLNOERROR;
}

ECODE WINAPI olDaGetDASS (HDEV hDev, OLSS OlSs, UINT uiElement, PHDASS phDass) {
    return OLNOERROR;
}

ECODE WINAPI olDaGetEncoding (HDASS hDass, PUINT puiEnc) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetDataFlow(HDASS hDass, UINT uiDataFlow) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetWrapMode (HDASS hDass, UINT uiWrapMode) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetClockFrequency (HDASS hDass, DBL dfFreq) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetDmaUsage (HDASS hDass, UINT uiUse) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetRetriggerFrequency (HDASS hDass, DBL dfFreq) {
    return OLNOERROR;
}

ECODE WINAPI olDaConfig (HDASS hDass) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetChannelListSize (HDASS hDass, UINT uiSize) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetChannelListEntry (HDASS hDass, UINT uiEntry, UINT uiChan) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetGainListEntry (HDASS hDass, UINT uiEntry, DBL dGain) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetChannelFilter (HDASS hDass, UINT uiChan, DBL dCutOffFreq) {
    return OLNOERROR;
}

ECODE WINAPI olDaSetRange (HDASS hDass, DBL dMaxVoltage, DBL dMinVoltage) {
    device.max = dMaxVoltage;
    device.min = dMinVoltage;
    return OLNOERROR;
}

ECODE WINAPI olDmFreeBuffer (HBUF) {
    return OLNOERROR;
}

ECODE WINAPI olDaGetResolution (HDASS hDass, PUINT puiBits) {
    *puiBits = 32;
    return OLNOERROR;
}

ECODE WINAPI olDaGetChannelListSize (HDASS hDass, PUINT puiSize) {
    return OLNOERROR;
}

ECODE WINAPI olDmCallocBuffer (UINT i, UINT j, DWORD k, UINT l, LPHBUF hBuf) {
    HBUF buf;
    *hBuf = buf;
    return OLNOERROR;
}

ECODE WINAPI olDaPutBuffer (HDASS hDass, HBUF hBuf) {
    return OLNOERROR;
}

ECODE WINAPI olDmGetValidSamples (HBUF hBuf, DWORD* hPtr) {
    *hPtr = 1000;
    return OLNOERROR;
}

ECODE WINAPI olDmGetBufferPtr (HBUF hBuf, LPVOID* hPtr) {
    *hPtr = &device.buffer;
    return OLNOERROR;
}

void UpdateBuffer() {
    for (unsigned int i = 0; i < 100; i++) {
        device.buffer[device.i] = device.min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(device.max - device.min)));
        device.i = (device.i + 1) % 1000;
    }
}

ECODE WINAPI olDaStart (HDASS hDass) {
    for (unsigned int i = 0; i < 10; i++) {
        UpdateBuffer();
    }
    device.timer.setInterval(100);
    device.i = 0;
    device.connection = QObject::connect(&device.timer, &QTimer::timeout, UpdateBuffer);
    device.timer.start();
    return OLNOERROR;
}

ECODE WINAPI olDaAbort (HDASS hDass) {
    QObject::disconnect(device.connection);
    device.timer.stop();
    return OLNOERROR;
}

ECODE WINAPI olDaStop (HDASS hDass) {
    QObject::disconnect(device.connection);
    device.timer.stop();
    return OLNOERROR;
}

ECODE WINAPI olDaFlushBuffers (HDASS hDass) {
    return OLNOERROR;
}

ECODE WINAPI olDaGetBuffer (HDASS hDass, PHBUF phBuf) {
    return OLNOERROR;
}

ECODE WINAPI olDaReleaseDASS (HDASS hDass) {
    return OLNOERROR;
}

ECODE WINAPI olDaTerminate (HDEV hDev) {
    return OLNOERROR;
}
