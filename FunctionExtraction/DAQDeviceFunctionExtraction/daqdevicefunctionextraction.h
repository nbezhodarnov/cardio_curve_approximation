#ifndef DAQDEVICEFUNCTIONEXTRACTION_H
#define DAQDEVICEFUNCTIONEXTRACTION_H

#include <QWidget>

#include "function.h"
#include "FunctionExtraction/abstractfunctionextraction.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/olmem.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/oldaapi.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/oldadefs.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/olerrors.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionresources.h"

class DAQDeviceFunctionExtraction : public AbstractFunctionExtraction
{
public:
    DAQDeviceFunctionExtraction(QWidget *ptr = nullptr);
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
    Function extract();

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
    QList<FunctionElement> function;
};

#endif // DAQDEVICEFUNCTIONEXTRACTION_H
