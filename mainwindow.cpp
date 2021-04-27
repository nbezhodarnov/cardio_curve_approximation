#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QTextStream>
#include <QFileDialog>

#include "FunctionExtraction/filefunctionextractionmanager.h"
#include "FunctionApproximator/NetFunctionApproximator/netfunctionapproximator.h"
#ifdef _WIN32
#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextraction.h"
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    approximation(0, 0, 0, 0, 0, 0, 0)
{
    ui->setupUi(this);
    ui->widget->setVisible(false);
    ui->widget->setEnabled(false);

    /*
    ui->extractor_choose->lineEdit()->setAlignment(Qt::AlignCenter);
    for (int i = 0; i < ui->extractor_choose->count(); i++) {
        ui->extractor_choose->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }
    ui->extractor_choose->lineEdit()->setReadOnly(true);
    */

    extractor = new FileFunctionExtractionManager(this);
    approximator = new NetFunctionApproximator(this);
#ifndef _WIN32
    ui->extractor_choose->removeItem(1);
#endif
}

void MainWindow::on_rangeSelectionStateChanged(bool selected) {
    ui->approximation_start->setEnabled(selected);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete extractor;
    delete approximator;

    for (unsigned int i = 0; i < (unsigned int)results_windows.size(); i++) {
        delete results_windows[i];
    }
}

void MainWindow::on_make_plot_clicked()
{
    Function function = extractor->extract();
    if (function == Function()) {
        return;
    }

    ui->widget->setEnabled(true);
    ui->widget->setVisible(true);
    ui->widget->setFunction(function);
    connect(ui->widget, SIGNAL(rangeSelectionStateChanged(bool)), this, SLOT(on_rangeSelectionStateChanged(bool)));
}

void MainWindow::on_approximation_start_clicked()
{
    Function data = ui->widget->getSelectedRange();
    approximation = approximator->approximate(data);

    unsigned int n = 100;
    Function result, e_first, e_second;
    double step = (data.getKey(data.size() - 1) - data.getKey(0)) / (double)(n - 1);
    for (quint8 i = 0; i < n; i++) {
        double x = data.getKey(0) + i * step;
        double e_first_value = approximation.getFirstComponentValue(x);
        double e_second_value = approximation.getSecondComponentValue(x);
        double result_value = e_first_value + e_second_value + approximation.getConstant();
        e_first_value += approximation.getConstant();
        e_second_value += approximation.getConstant();
        result.add(x, result_value);
        e_first.add(x, e_first_value);
        e_second.add(x, e_second_value);
    }
    ui->widget->plotFunction(result, QPen(QBrush(QColor(Qt::magenta)), 1));
    ui->widget->plotFunction(e_first, QPen(QBrush(QColor(Qt::darkYellow)), 1));
    ui->widget->plotFunction(e_second, QPen(QBrush(QColor(Qt::darkGreen)), 1));

    ui->results->setEnabled(true);
}

void MainWindow::on_results_clicked()
{
    results_windows.push_back(new FunctionApproximationTabWidget(nullptr, approximation));
    results_windows[results_windows.size() - 1]->show();
}

void MainWindow::on_extractor_choose_currentIndexChanged(int index)
{
    delete extractor;
    switch (index) {
    case 0: {
        extractor = new FileFunctionExtractionManager(this);
        break;
    }
#ifdef _WIN32
    case 1: {
        extractor = new DAQDeviceFunctionExtraction(this);
        break;
    }
#endif
    default: {
        extractor = new FileFunctionExtractionManager(this);
        break;
    }
    }
}
