#ifndef DAQDEVICEINTERFACE_H
#define DAQDEVICEINTERFACE_H

#include <QStringList>
#include <QList>

#include "function.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/olmem.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/oldaapi.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/oldadefs.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/olerrors.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionresources.h"

class DAQDeviceInterface
{
public:
    DAQDeviceInterface() = default;
    QStringList getBoardsList();
    bool connect(const QString&);
    unsigned int getChannelsCount();
    QStringList getGainsList();
    QStringList getRangesList();
    QStringList getFiltersList();
    double getFrequency();
    Range getRange();
    bool setParameters(unsigned int, double, Range, unsigned int);
    bool start();
    QList<FunctionElement> getData();
    void abort();
    void stop();
    void disconnect();

private:
    Board board;
    Range rangeBoard;
    double frequency;
    double gainBoard;
    double x;
    unsigned int channelsCount;
    unsigned int encoding;
    unsigned int bReturnsFloats;
    unsigned int resolution;
    unsigned int listsize;
    unsigned int gainsup;
};

#endif // DAQDEVICEINTERFACE_H
