#include <QMessageBox>

#include "daqdevicefunctionextractionwizardpage1choosedevice.h"
#include "ui_daqdevicefunctionextractionwizardpage1choosedevice.h"

DAQDeviceFunctionExtractionWizardPage1ChooseDevice::DAQDeviceFunctionExtractionWizardPage1ChooseDevice(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::DAQDeviceFunctionExtractionWizardPage1ChooseDevice),
    ptr(nullptr)
{
    ui->setupUi(this);
}

DAQDeviceFunctionExtractionWizardPage1ChooseDevice::~DAQDeviceFunctionExtractionWizardPage1ChooseDevice()
{
    delete ui;
}

void DAQDeviceFunctionExtractionWizardPage1ChooseDevice::UpdateDevicesList(const QStringList &devices)
{
    ui->deviceComboBox->clear();
    ui->deviceComboBox->addItems(devices);
    emit completeChanged();
}

QString DAQDeviceFunctionExtractionWizardPage1ChooseDevice::GetDeviceName()
{
    return ui->deviceComboBox->currentText();
}

void DAQDeviceFunctionExtractionWizardPage1ChooseDevice::SetDAQDevicePtr(DAQDeviceInterface *input)
{
    ptr = input;
}

bool DAQDeviceFunctionExtractionWizardPage1ChooseDevice::isComplete() const {
    return ui->deviceComboBox->currentIndex() != -1;
}

void DAQDeviceFunctionExtractionWizardPage1ChooseDevice::on_deviceUpdateButton_clicked()
{
    if (ptr != nullptr) {
        UpdateDevicesList(ptr->getBoardsList());
    }
}
