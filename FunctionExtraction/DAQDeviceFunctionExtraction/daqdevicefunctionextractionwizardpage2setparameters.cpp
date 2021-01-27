#include "daqdevicefunctionextractionwizardpage2setparameters.h"
#include "ui_daqdevicefunctionextractionwizardpage2setparameters.h"

DAQDeviceFunctionExtractionWizardPage2SetParameters::DAQDeviceFunctionExtractionWizardPage2SetParameters(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::DAQDeviceFunctionExtractionWizardPage2SetParameters)
{
    ui->setupUi(this);
}

DAQDeviceFunctionExtractionWizardPage2SetParameters::~DAQDeviceFunctionExtractionWizardPage2SetParameters()
{
    delete ui;
}

void DAQDeviceFunctionExtractionWizardPage2SetParameters::UpdateChannelComboBox(unsigned int count)
{
    ui->channelComboBox->clear();
    for (unsigned int i = 0; i < count; i++) {
        ui->channelComboBox->addItem(QString::number(i));
    }
    emit completeChanged();
}

void DAQDeviceFunctionExtractionWizardPage2SetParameters::UpdateGainComboBox(const QStringList &gains)
{
    ui->gainComboBox->clear();
    ui->gainComboBox->addItems(gains);
    emit completeChanged();
}

void DAQDeviceFunctionExtractionWizardPage2SetParameters::UpdateRangeComboBox(const QStringList &ranges)
{
    ui->rangeComboBox->clear();
    ui->rangeComboBox->addItems(ranges);
    emit completeChanged();
}

void DAQDeviceFunctionExtractionWizardPage2SetParameters::UpdateFilterComboBox(const QStringList &filters)
{
    ui->filterComboBox->clear();
    ui->filterComboBox->addItems(filters);
    emit completeChanged();
}

unsigned int DAQDeviceFunctionExtractionWizardPage2SetParameters::GetChannel()
{
    return ui->channelComboBox->currentIndex();
}

double DAQDeviceFunctionExtractionWizardPage2SetParameters::GetGain()
{
    return ui->gainComboBox->currentText().toDouble();
}

Range DAQDeviceFunctionExtractionWizardPage2SetParameters::GetRange()
{
    QString range = ui->rangeComboBox->currentText();
    if (range.size() == 0) {
        return {0, 0};
    }
    unsigned int firstStart = 0, firstEnd = 0, secondStart = 0, secondEnd = 0;
    while ((range[firstStart] != '-') && ((range[firstStart] < '0') || (range[firstStart] > '9'))) {
        firstStart++;
    }
    firstEnd = firstStart;
    while (range[firstEnd + 1] != 'V') {
        firstEnd++;
    }
    secondStart = firstEnd + 2;
    while (((range[secondStart] != '-') || (range[secondStart + 1] == ' ')) && ((range[secondStart] < '0') || (range[secondStart] > '9'))) {
        secondStart++;
    }
    secondEnd = secondStart;
    while (range[secondEnd + 1] != 'V') {
        secondEnd++;
    }
    return {range.mid(firstStart, firstEnd - firstStart + 1).toDouble(), range.mid(secondStart, secondEnd - secondStart + 1).toDouble()};
}

unsigned int DAQDeviceFunctionExtractionWizardPage2SetParameters::GetFilter()
{
    QString filter = ui->filterComboBox->currentText();
    filter.chop(2);
    return filter.toUInt();
}

bool DAQDeviceFunctionExtractionWizardPage2SetParameters::isComplete() const {
    return ((ui->channelComboBox->currentIndex() != -1) && (ui->gainComboBox->currentIndex() != -1) && (ui->rangeComboBox->currentIndex() != -1) && (ui->filterComboBox->currentIndex() != -1));
}
