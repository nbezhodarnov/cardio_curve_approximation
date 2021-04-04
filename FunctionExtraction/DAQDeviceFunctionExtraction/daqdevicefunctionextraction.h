#ifndef DAQDEVICEFUNCTIONEXTRACTION_H
#define DAQDEVICEFUNCTIONEXTRACTION_H

#include <QWidget>

#include "function.h"
#include "FunctionExtraction/abstractfunctionextraction.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/olmem.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/oldaapi.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/oldadefs.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/olerrors.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdeviceinterface.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionresources.h"

class DAQDeviceFunctionExtraction : public AbstractFunctionExtraction
{
public:
    DAQDeviceFunctionExtraction(QWidget *ptr = nullptr);
    Function extract();

private:
    DAQDeviceInterface device_interface;
    QList<FunctionElement> function;
};

#endif // DAQDEVICEFUNCTIONEXTRACTION_H
