#include "daqdevicefunctionextractionwizardpage3getdata.h"
#include "ui_daqdevicefunctionextractionwizardpage3getdata.h"

DAQDeviceFunctionExtractionWizardPage3GetData::DAQDeviceFunctionExtractionWizardPage3GetData(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::DAQDeviceFunctionExtractionWizardPage3GetData),
    ptr(nullptr),
    endButtonClicked(false),
    data(nullptr)
{
    ui->setupUi(this);

    getDataTimer.setInterval(1000);
    printDataTimer.setInterval(25);

    ui->plotBuilder->clearGraphs();
    //ui->plotBuilder->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
    ui->plotBuilder->addGraph();
    ui->plotBuilder->xAxis->setLabel("x");
    ui->plotBuilder->yAxis->setLabel("y");
    ui->plotBuilder->xAxis->setRange(0, 1);

    print_plot = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
}

DAQDeviceFunctionExtractionWizardPage3GetData::~DAQDeviceFunctionExtractionWizardPage3GetData()
{
    delete ui;
}

void DAQDeviceFunctionExtractionWizardPage3GetData::SetDAQDevicePtr(DAQDeviceInterface *board)
{
    ptr = board;
    getDataTimer.setInterval(ptr->getFrequency());
    printDataTimer.setInterval(ptr->getFrequency() * 0.025);
}

void DAQDeviceFunctionExtractionWizardPage3GetData::SetDataPtr(QList<FunctionElement> *func)
{
    data = func;
}

void DAQDeviceFunctionExtractionWizardPage3GetData::UpdateRange()
{
    if (ptr != nullptr) {
        Range range = ptr->getRange();
        ui->plotBuilder->yAxis->setRange(range.min - 1, range.max + 1);
        ui->plotBuilder->replot();
    }
}

bool DAQDeviceFunctionExtractionWizardPage3GetData::isComplete() const
{
    return ((endButtonClicked) && (data != nullptr) && (!data->empty()));
}

void DAQDeviceFunctionExtractionWizardPage3GetData::on_startPushButton_clicked()
{
    if (ptr != nullptr) {
        endButtonClicked = false;
        ptr->start();
        ui->endPushButton->setEnabled(true);
        ui->startPushButton->setEnabled(false);
        connect(&getDataTimer, SIGNAL(timeout()), this, SLOT(GetData()));
        connect(&printDataTimer, SIGNAL(timeout()), this, SLOT(PrintData()));
        getDataTimer.start();
        printDataTimer.start();
    }
}

void DAQDeviceFunctionExtractionWizardPage3GetData::on_endPushButton_clicked()
{
    if (ptr != nullptr) {
        ptr->stop();
        StopEvent();
    }
}

void DAQDeviceFunctionExtractionWizardPage3GetData::GetData()
{
    if ((ptr != nullptr) && (data != nullptr)) {
        //int previousSize = temp.size();
        temp.append(ptr->getData());
        /*if (temp.size() == previousSize) {
            ptr->abort();
            StopEvent();
        }*/
    }
}

void DAQDeviceFunctionExtractionWizardPage3GetData::PrintData()
{
    if ((ptr != nullptr) && (data != nullptr)) {
        QList<FunctionElement>::iterator dataIterator = temp.begin();
        for (unsigned int i = 0; (dataIterator != temp.end()) && (i < (unsigned int)(ptr->getFrequency() * 0.025)); dataIterator = temp.erase(dataIterator), i++) {
            print_plot->add({dataIterator->x, dataIterator->f});
            if (dataIterator->x > 1) {
                print_plot->remove(0, dataIterator->x - 1.0);
                ui->plotBuilder->xAxis->setRange(dataIterator->x - 1, dataIterator->x);
            }
            ui->plotBuilder->graph(0)->setData(print_plot);
            ui->plotBuilder->replot();
            data->append(*dataIterator);
        }
    }
}

void DAQDeviceFunctionExtractionWizardPage3GetData::StopEvent()
{
    disconnect(&getDataTimer, SIGNAL(timeout()), this, SLOT(GetData()));
    disconnect(&printDataTimer, SIGNAL(timeout()), this, SLOT(PrintData()));
    getDataTimer.stop();
    printDataTimer.stop();
    ui->endPushButton->setEnabled(false);
    ui->startPushButton->setEnabled(true);
    if (!data->empty()) {
        endButtonClicked = true;
        setCommitPage(true);
    }
}
