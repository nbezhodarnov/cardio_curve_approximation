#ifndef DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE4WRITETOFILE_H
#define DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE4WRITETOFILE_H

#include <QWizardPage>

#include "functionelement.h"

namespace Ui {
class DAQDeviceFunctionExtractionWizardPage4WriteToFile;
}

class DAQDeviceFunctionExtractionWizardPage4WriteToFile : public QWizardPage
{
    Q_OBJECT

public:
    explicit DAQDeviceFunctionExtractionWizardPage4WriteToFile(QWidget *parent = nullptr);
    ~DAQDeviceFunctionExtractionWizardPage4WriteToFile();
    void SetFunction(QList<FunctionElement>*);
    bool isComplete() const;

private slots:
    void on_fileNamePushButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::DAQDeviceFunctionExtractionWizardPage4WriteToFile *ui;
    QList<FunctionElement> *function;
    bool saved;
};

#endif // DAQDEVICEFUNCTIONEXTRACTIONWIZARDPAGE4WRITETOFILE_H
