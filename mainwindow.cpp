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

    QVector<double> coefficients = approximation.getCoefficients();
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(QString::fromUtf8("Результаты"));
    msgBox.setText(QString::fromUtf8("Результаты вычислений:\n") +
                   QString::fromUtf8("Коэффициенты при экспонентах:\na1 = ") + QString::number(coefficients[0]) +
            QString::fromUtf8(", b1 = ") + QString::number(coefficients[1]) +
            QString::fromUtf8(", c1 = ") + QString::number(coefficients[2]) +
            QString::fromUtf8("\na2 = ") + QString::number(coefficients[3]) +
            QString::fromUtf8(", b2 = ") + QString::number(coefficients[4]) +
            QString::fromUtf8(", c2 = ") + QString::number(coefficients[5]) +
            QString::fromUtf8("\nПервая производная:\n") +
            QString::number(-2 * coefficients[0] * coefficients[1]) +
            QString::fromUtf8("(x-") + QString::number(coefficients[2]) + QString::fromUtf8(")exp{") +
            QString::number(-coefficients[1]) + QString::fromUtf8("(x-") + QString::number(coefficients[2]) + QString::fromUtf8(")^2}") +
            QString::number(-2 * coefficients[3] * coefficients[4]) +
            QString::fromUtf8("(x-") + QString::number(coefficients[5]) + QString::fromUtf8(")exp{") +
            QString::number(-coefficients[4]) + QString::fromUtf8("(x-") + QString::number(coefficients[5]) +
            QString::fromUtf8(")^2}") +
            QString::fromUtf8("\nВторая производная:\n") +
            QString::number(-2 * coefficients[0] * coefficients[1]) +
            QString::fromUtf8("exp{") + QString::number(-coefficients[1]) +
            QString::fromUtf8("(x-") + QString::number(coefficients[2]) + QString::fromUtf8(")^2}+") +
            QString::number(4 * coefficients[0] * pow(coefficients[1], 2)) +
            QString::fromUtf8("(x-") + QString::number(coefficients[2]) + QString::fromUtf8(")^(2)exp{") +
            QString::number(-coefficients[1]) + QString::fromUtf8("(x-") + QString::number(coefficients[2]) + QString::fromUtf8(")^2}\n") +
            QString::number(-2 * coefficients[3] * coefficients[4]) +
            QString::fromUtf8("exp{") + QString::number(-coefficients[4]) +
            QString::fromUtf8("(x-") + QString::number(coefficients[5]) + QString::fromUtf8(")^2}+") +
            QString::number(4 * coefficients[3] * pow(coefficients[4], 2)) +
            QString::fromUtf8("(x-") + QString::number(coefficients[5]) + QString::fromUtf8(")^(2)exp{") +
            QString::number(-coefficients[4]) + QString::fromUtf8("(x-") + QString::number(coefficients[5]) + QString::fromUtf8(")^2}"));
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.exec();
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
