#ifndef DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE3GETDATA_H
#define DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE3GETDATA_H

#include <QTimer>
#include <QWizardPage>

#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextraction.h"
#include "qcustomplot.h"

namespace Ui {
class DAQDeviceFunctionExtractionWizardPage3GetData;
}

class DAQDeviceFunctionExtractionWizardPage3GetData : public QWizardPage
{
    Q_OBJECT

public:
    explicit DAQDeviceFunctionExtractionWizardPage3GetData(QWidget *parent = nullptr);
    ~DAQDeviceFunctionExtractionWizardPage3GetData();
    void SetDAQDevicePtr(DAQDeviceInterface*);
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
    DAQDeviceInterface *ptr;
    bool endButtonClicked;
    QList<FunctionElement> *data;
    QList<FunctionElement> temp;
    QTimer getDataTimer;
    QTimer printDataTimer;
    QSharedPointer<QCPGraphDataContainer> print_plot;
};

#endif // DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE3GETDATA_H
