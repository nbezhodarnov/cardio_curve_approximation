#ifndef DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE3GETDATA_H
#define DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE3GETDATA_H

#include <QTimer>
#include <QWizardPage>

#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextraction.h"

namespace Ui {
class DAQDeviceFunctionExtractionWizardPage3GetData;
}

class DAQDeviceFunctionExtractionWizardPage3GetData : public QWizardPage
{
    Q_OBJECT

public:
    explicit DAQDeviceFunctionExtractionWizardPage3GetData(QWidget *parent = nullptr);
    ~DAQDeviceFunctionExtractionWizardPage3GetData();
    void SetDAQDevicePtr(DAQDeviceFunctionExtraction*);
    void SetDataPtr(QList<FunctionElement>*);
    void UpdateRange();
    bool isComplete() const;

private slots:
    void on_startPushButton_clicked();

    void on_endPushButton_clicked();

    void GetData();

    void PrintData();

private:
    void StopEvent();

    Ui::DAQDeviceFunctionExtractionWizardPage3GetData *ui;
    DAQDeviceFunctionExtraction *ptr;
    bool endButtonClicked;
    QList<FunctionElement> *data;
    QList<FunctionElement> temp;
    QTimer getDataTimer;
    QTimer printDataTimer;
};

#endif // DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE3GETDATA_H
