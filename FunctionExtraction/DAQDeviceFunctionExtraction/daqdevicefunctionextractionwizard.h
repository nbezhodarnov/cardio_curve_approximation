#ifndef DAQDEVICEFUNCTIONEXTRACTIONWIZARD_H
#define DAQDEVICEFUNCTIONEXTRACTIONWIZARD_H

#include <QWizard>

#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextraction.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionresources.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage1choosedevice.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage2setparameters.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage3getdata.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage4writetofile.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage5finish.h"

namespace Ui {
class DAQDeviceFunctionExtractionWizard;
}

class DAQDeviceFunctionExtractionWizard : public QWizard
{
    Q_OBJECT

public:
    explicit DAQDeviceFunctionExtractionWizard(QWidget *parent = nullptr);
    ~DAQDeviceFunctionExtractionWizard();
    void SetDAQDevicePtr(DAQDeviceInterface*);
    void SetFunctionPtr(QList<FunctionElement>*);

private slots:
    void on_DAQDeviceFunctionExtractionWizard_currentIdChanged(int id);

private:
    Ui::DAQDeviceFunctionExtractionWizard *ui;
    DAQDeviceInterface *boardInterface;
    unsigned int previousPageId;
    QList<FunctionElement> *function;
};

#endif // DAQDEVICEFUNCTIONEXTRACTIONWIZARD_H
