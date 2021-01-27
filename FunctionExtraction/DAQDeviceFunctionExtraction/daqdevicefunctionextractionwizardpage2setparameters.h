#ifndef DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE2SETPARAMETERS_H
#define DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE2SETPARAMETERS_H

#include <QWizardPage>

#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionresources.h"

namespace Ui {
class DAQDeviceFunctionExtractionWizardPage2SetParameters;
}

class DAQDeviceFunctionExtractionWizardPage2SetParameters : public QWizardPage
{
    Q_OBJECT

public:
    explicit DAQDeviceFunctionExtractionWizardPage2SetParameters(QWidget *parent = nullptr);
    ~DAQDeviceFunctionExtractionWizardPage2SetParameters();
    void UpdateChannelComboBox(unsigned int);
    void UpdateGainComboBox(const QStringList&);
    void UpdateRangeComboBox(const QStringList&);
    void UpdateFilterComboBox(const QStringList&);
    unsigned int GetChannel();
    double GetGain();
    Range GetRange();
    unsigned int GetFilter();
    bool isComplete() const;


private:
    Ui::DAQDeviceFunctionExtractionWizardPage2SetParameters *ui;
};

#endif // DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE2SETPARAMETERS_H
