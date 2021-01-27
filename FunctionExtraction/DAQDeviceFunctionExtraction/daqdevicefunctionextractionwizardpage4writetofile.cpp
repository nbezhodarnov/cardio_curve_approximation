#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include "daqdevicefunctionextractionwizardpage4writetofile.h"
#include "ui_daqdevicefunctionextractionwizardpage4writetofile.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionresources.h"

DAQDeviceFunctionExtractionWizardPage4WriteToFile::DAQDeviceFunctionExtractionWizardPage4WriteToFile(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::DAQDeviceFunctionExtractionWizardPage4WriteToFile),
    function(nullptr),
    saved(false)
{
    ui->setupUi(this);
}

DAQDeviceFunctionExtractionWizardPage4WriteToFile::~DAQDeviceFunctionExtractionWizardPage4WriteToFile()
{
    delete ui;
}

void DAQDeviceFunctionExtractionWizardPage4WriteToFile::SetFunction(QList<FunctionElement> *func)
{
    function = func;
}

bool DAQDeviceFunctionExtractionWizardPage4WriteToFile::isComplete() const
{
    return saved;
}

void DAQDeviceFunctionExtractionWizardPage4WriteToFile::on_fileNamePushButton_clicked()
{
    ui->fileNameLineEdit->setText(QFileDialog::getSaveFileName(this, "Сохранение файла", "", "Файлы .ndat (*.ndat)"));
}

void DAQDeviceFunctionExtractionWizardPage4WriteToFile::on_saveButton_clicked()
{
    if (function == nullptr) {
        return;
    }
    QFile file(ui->fileNameLineEdit->text());
    if (!file.open(QIODevice::WriteOnly)) {
        DisplayErrorMessage("Ошибки при сохранении файла!");
        return;
    }
    QTextStream out(&file);
    for (QList<FunctionElement>::iterator iterator = function->begin(); iterator != function->end(); iterator++) {
        out << QString::number(iterator->x, 'g', 8) + " " + QString::number(iterator->f, 'g', 8) + '\n';
        if (out.status() != QTextStream::Status::Ok) {
            DisplayErrorMessage("Ошибки при сохранении файла!");
            return;
        }
    }
    file.flush();
    file.close();
    saved = true;
    setCommitPage(true);
    ui->successLabel->setText("Файл успешно сохранён!");
}
