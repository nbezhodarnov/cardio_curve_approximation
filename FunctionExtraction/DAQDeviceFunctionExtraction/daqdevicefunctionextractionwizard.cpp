#include "daqdevicefunctionextractionwizard.h"
#include "ui_daqdevicefunctionextractionwizard.h"

DAQDeviceFunctionExtractionWizard::DAQDeviceFunctionExtractionWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::DAQDeviceFunctionExtractionWizard),
    boardInterface(nullptr),
    previousPageId(0),
    function(nullptr)
{
    ui->setupUi(this);
}

DAQDeviceFunctionExtractionWizard::~DAQDeviceFunctionExtractionWizard()
{
    delete ui;
    if (boardInterface != nullptr) {
        boardInterface->disconnect();
    }
}

void DAQDeviceFunctionExtractionWizard::SetDAQDevicePtr(DAQDeviceInterface *boardInterfaceInput)
{
    boardInterface = boardInterfaceInput;
    ui->wizardPage1ChooseDevice->SetDAQDevicePtr(boardInterface);
    ui->wizardPage3GetData->SetDAQDevicePtr(boardInterface);
}

void DAQDeviceFunctionExtractionWizard::SetFunctionPtr(QList<FunctionElement> *ptr)
{
    function = ptr;
}

void DAQDeviceFunctionExtractionWizard::on_DAQDeviceFunctionExtractionWizard_currentIdChanged(int id)
{
    if ((boardInterface == nullptr) || (function == nullptr)) {
        return;
    }
    switch (id) {
    case 0: {
        if (previousPageId != 0) {
            boardInterface->disconnect();
        }
        previousPageId = 0;
        ui->wizardPage1ChooseDevice->UpdateDevicesList(boardInterface->getBoardsList());
        break;
    }
    case 1: {
        if (previousPageId == 2) {
            previousPageId = 1;
            break;
        }
        previousPageId = 1;
        if (boardInterface->connect(ui->wizardPage1ChooseDevice->GetDeviceName()) == false) {
            boardInterface->disconnect();
            back();
            break;
        }
        ui->wizardPage2SetParameters->UpdateChannelComboBox(boardInterface->getChannelsCount());
        ui->wizardPage2SetParameters->UpdateGainComboBox(boardInterface->getGainsList());
        ui->wizardPage2SetParameters->UpdateRangeComboBox(boardInterface->getRangesList());
        ui->wizardPage2SetParameters->UpdateFilterComboBox(boardInterface->getFiltersList());
        break;
    }
    case 2: {
        if (previousPageId == 3) {
            previousPageId = 2;
            break;
        }
        previousPageId = 2;
        unsigned int channel = ui->wizardPage2SetParameters->GetChannel();
        double gain = ui->wizardPage2SetParameters->GetGain();
        Range range = ui->wizardPage2SetParameters->GetRange();
        unsigned int filter = ui->wizardPage2SetParameters->GetFilter();
        if (boardInterface->setParameters(channel, gain, range, filter) == false) {
            boardInterface->disconnect();
            back();
            back();
        }
        ui->wizardPage3GetData->SetDataPtr(function);
        ui->wizardPage3GetData->UpdateRange();
        break;
    }
    case 3: {
        previousPageId = 3;
        ui->wizardPage4WriteToFile->SetFunction(function);
        boardInterface->disconnect();
        break;
    }
    default:
        break;
    }
}
