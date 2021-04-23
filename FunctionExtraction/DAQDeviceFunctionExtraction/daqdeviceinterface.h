#ifndef DAQDEVICEINTERFACE_H
#define DAQDEVICEINTERFACE_H

#include <QStringList>
#include <QWindow>
#include <QList>

#include "function.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/olmem.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/oldaapi.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/oldadefs.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/olerrors.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionresources.h"

void __stdcall extractData(unsigned int, unsigned int, long);

class DAQDeviceInterface : public QWindow
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

    void addData(const QList<FunctionElement>&);
    Board& getBoard();
    unsigned int getEncoding();
    unsigned int DoReturnFloats();
    unsigned int getResolution();
    unsigned int getListSize();
    unsigned int getGainBoard();
    double& getX();

protected:
    bool nativeEvent(const QByteArray&, void*, long*);

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
    QList<FunctionElement> data;
};

#endif // DAQDEVICEINTERFACE_H
