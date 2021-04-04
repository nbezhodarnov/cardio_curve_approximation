#ifndef DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE1CHOOSEDEVICE_H
#define DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE1CHOOSEDEVICE_H

#include <QWizardPage>

#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizard.h"

namespace Ui {
class DAQDeviceFunctionExtractionWizardPage1ChooseDevice;
}

class DAQDeviceFunctionExtractionWizardPage1ChooseDevice : public QWizardPage
{
    Q_OBJECT

public:
    explicit DAQDeviceFunctionExtractionWizardPage1ChooseDevice(QWidget *parent = nullptr);
    ~DAQDeviceFunctionExtractionWizardPage1ChooseDevice();
    void UpdateDevicesList(const QStringList&);
    QString GetDeviceName();
    void SetDAQDevicePtr(DAQDeviceInterface*);
    bool isComplete() const;

private slots:
    void on_deviceUpdateButton_clicked();

private:
    Ui::DAQDeviceFunctionExtractionWizardPage1ChooseDevice *ui;
    DAQDeviceInterface *ptr;
};

#endif // DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE1CHOOSEDEVICE_H
