#include "daqdevicefunctionextractionwizardpage5finish.h"
#include "ui_daqdevicefunctionextractionwizardpage5finish.h"

DAQDeviceFunctionExtractionWizardPage5Finish::DAQDeviceFunctionExtractionWizardPage5Finish(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::DAQDeviceFunctionExtractionWizardPage5Finish)
{
    ui->setupUi(this);
}

DAQDeviceFunctionExtractionWizardPage5Finish::~DAQDeviceFunctionExtractionWizardPage5Finish()
{
    delete ui;
}
