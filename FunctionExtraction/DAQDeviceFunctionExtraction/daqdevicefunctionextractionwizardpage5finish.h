#ifndef DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE5FINISH_H
#define DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE5FINISH_H

#include <QWizardPage>

namespace Ui {
class DAQDeviceFunctionExtractionWizardPage5Finish;
}

class DAQDeviceFunctionExtractionWizardPage5Finish : public QWizardPage
{
    Q_OBJECT

public:
    explicit DAQDeviceFunctionExtractionWizardPage5Finish(QWidget *parent = nullptr);
    ~DAQDeviceFunctionExtractionWizardPage5Finish();

private:
    Ui::DAQDeviceFunctionExtractionWizardPage5Finish *ui;
};

#endif // DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE5FINISH_H
