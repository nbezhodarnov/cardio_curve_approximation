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

    for (unsigned int i = 0; i < (unsigned int)analysis_windows.size(); i++) {
        delete analysis_windows[i];
    }
}

void MainWindow::on_make_plot_clicked()
{
    Function function = extractor->extract();
    if (function == Function()) {
        return;
    }
    experiment = function;

    ui->analyze_start->setEnabled(true);
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
    ui->save_results_to_file->setEnabled(true);
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

#define PRECISION 0.001

void MainWindow::on_save_results_to_file_clicked()
{
    QString file_name = QFileDialog::getSaveFileName(this, tr("Сохранить результаты"), "", tr("Результаты (*.nresult);;Все файлы (*)"));
    if (file_name.isEmpty()) {
        //std::cerr << "No file name has been provided!\n";
        return;
    }
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString::fromUtf8("Ошибка"));
        msgBox.setText(QString::fromUtf8("Произошла ошибка при записи результатов в файл!"));
        msgBox.exec();
        return;
    }
    QVector<double> coefficients = approximation.getCoefficients();
    bool are_two_peaks = true;
    if (coefficients[3] == 0) {
        are_two_peaks = false;
    }
    double constant = approximation.getConstant();
    double start = findKeyByValue([this](const double &key) {return approximation.getValue(key);}, constant, PRECISION, coefficients[2]);
    double end = findKeyByValue([this](const double &key) {return approximation.getValue(key);}, constant, PRECISION, coefficients[5]);
    if (!are_two_peaks) {
        end = findKeyByValue([this](const double &key) {return approximation.getValue(key);}, constant, PRECISION, coefficients[2] + 0.5);
    }

    QString result =
            QString::fromUtf8("Результаты вычислений:\n") +
            QString::fromUtf8("Коэффициенты при экспонентах:\na1 = ") + QString::number(coefficients[0]) +
            QString::fromUtf8(", b1 = ") + QString::number(coefficients[1]) +
            QString::fromUtf8(", c1 = ") + QString::number(coefficients[2]) +
            QString::fromUtf8("\na2 = ") + QString::number(coefficients[3]) +
            QString::fromUtf8(", b2 = ") + QString::number(coefficients[4]) +
            QString::fromUtf8(", c2 = ") + QString::number(coefficients[5]) +
            QString::fromUtf8("\nconst = ") + QString::number(coefficients[6]) +
            QString::fromUtf8("\nПервая производная:\n") +
            QString::number(-2 * coefficients[0] * coefficients[1]) +
            QString::fromUtf8("(x-") +
            QString::number(coefficients[2]) +
            QString::fromUtf8(")exp{") +
            QString::number(-coefficients[1]) +
            QString::fromUtf8("(x-") +
            QString::number(coefficients[2]) +
            QString::fromUtf8(")^2}") +
            QString::number(-2 * coefficients[3] * coefficients[4]) +
            QString::fromUtf8("(x-") + QString::number(coefficients[5]) +
            QString::fromUtf8(")exp{") +
            QString::number(-coefficients[4]) +
            QString::fromUtf8("(x-") +
            QString::number(coefficients[5]) +
            QString::fromUtf8(")^2}") +
            QString::fromUtf8("\nВторая производная:\n") +
            QString::fromUtf8("2(2*") +
            QString::number(coefficients[0]) +
            QString::fromUtf8("*") +
            QString::number(coefficients[1]) +
            QString::fromUtf8("^(2)*(x-") +
            QString::number(coefficients[2]) +
            QString::fromUtf8(")^(2)*") +
            QString::fromUtf8("exp{") +
            QString::number(-coefficients[1]) +
            QString::fromUtf8("(x-") +
            QString::number(coefficients[2]) +
            QString::fromUtf8(")^2}") +
            QString::fromUtf8("-") +
            QString::number(coefficients[0]) +
            QString::fromUtf8("*") +
            QString::number(coefficients[1]) +
            QString::fromUtf8("*exp{") +
            QString::number(-coefficients[1]) +
            QString::fromUtf8("(x-") +
            QString::number(coefficients[2]) +
            QString::fromUtf8(")^2}") +
            QString::fromUtf8("+2*") +
            QString::number(coefficients[3]) +
            QString::fromUtf8("*") +
            QString::number(coefficients[4]) +
            QString::fromUtf8("^(2)*(x-") +
            QString::number(coefficients[5]) +
            QString::fromUtf8(")^(2)*") +
            QString::fromUtf8("exp{") +
            QString::number(-coefficients[4]) +
            QString::fromUtf8("(x-") +
            QString::number(coefficients[5]) +
            QString::fromUtf8(")^2}") +
            QString::fromUtf8("-") +
            QString::number(coefficients[3]) +
            QString::fromUtf8("*") +
            QString::number(coefficients[4]) +
            QString::fromUtf8("*exp{") +
            QString::number(-coefficients[4]) +
            QString::fromUtf8("(x-") +
            QString::number(coefficients[5]) +
            QString::fromUtf8(")^2})");

    double peak_time_1 = 0, peak_time_2 = 0;
    peak_time_1 = findMaximum([this](const double &key){return approximation.getValue(key);}, PRECISION, coefficients[2]);
    peak_time_2 = findMaximum([this](const double &key){return approximation.getValue(key);}, PRECISION, coefficients[5]);
    if (are_two_peaks && std::abs(peak_time_2 - peak_time_1) < PRECISION * 50) {
        are_two_peaks = false;
        peak_time_2 = peak_time_1;
    } else if (!are_two_peaks) {
        peak_time_2 = peak_time_1;
    }
    double first_peak_maximum = approximation.getValue(peak_time_1), second_peak_maximim = -INFINITY;

    double amplitude = first_peak_maximum - constant;
    double amplitude_2 = first_peak_maximum - constant;

    double time_up_50_percent = -INFINITY;
    double time_down_50_percent = -INFINITY;

    double time_up_90_percent = -INFINITY;
    double time_down_90_percent = -INFINITY;

    double time_up_10_percent = -INFINITY;
    double time_down_10_percent = -INFINITY;

    QString additional_info_text = "";
    additional_info_text +=
            QString::fromUtf8("Момент первого пика: ") +
            QString::number(peak_time_1) +
            QString::fromUtf8("\nЗначение в первом пике: ") +
            QString::number(first_peak_maximum) + '\n';

    if (are_two_peaks) {
        second_peak_maximim = approximation.getValue(peak_time_2);
        additional_info_text +=
                QString::fromUtf8("Момент второго пика: ") +
                QString::number(peak_time_2) +
                QString::fromUtf8("\nЗначение во втором пике: ") +
                QString::number(second_peak_maximim) + '\n';
        amplitude_2 = second_peak_maximim - constant;
        time_down_50_percent = findKeyByValue([this](const double &key) {return approximation.getValue(key);}, second_peak_maximim - amplitude_2 / 2.0, PRECISION, end);
        time_down_90_percent = findKeyByValue([this](const double &key) {return approximation.getValue(key);}, second_peak_maximim - amplitude_2 / 10.0, PRECISION, end);
        time_down_10_percent = findKeyByValue([this](const double &key) {return approximation.getValue(key);}, second_peak_maximim - 9.0 * amplitude_2 / 10.0, PRECISION, end);
    } else {
        if (first_peak_maximum < second_peak_maximim) {
            peak_time_1 = peak_time_2;
            first_peak_maximum = second_peak_maximim;
            amplitude = first_peak_maximum - constant;
            amplitude_2 = first_peak_maximum - constant;
        }
        time_down_50_percent = findKeyByValue([this](const double &key) {return approximation.getValue(key);}, first_peak_maximum - amplitude / 2.0, PRECISION, end);
        time_down_90_percent = findKeyByValue([this](const double &key) {return approximation.getValue(key);}, first_peak_maximum - amplitude / 10.0, PRECISION, end);
        time_down_10_percent = findKeyByValue([this](const double &key) {return approximation.getValue(key);}, first_peak_maximum - 9.0 * amplitude / 10.0, PRECISION, end);
    }

    time_up_50_percent = findKeyByValue([this](const double &key) {return approximation.getValue(key);}, first_peak_maximum - amplitude / 2.0, PRECISION, start);

    time_up_90_percent = findKeyByValue([this](const double &key) {return approximation.getValue(key);}, first_peak_maximum - amplitude / 10.0, PRECISION, start);

    time_up_10_percent = findKeyByValue([this](const double &key) {return approximation.getValue(key);}, first_peak_maximum - 9.0 * amplitude / 10.0, PRECISION, start);

    additional_info_text +=
            QString::fromUtf8("Значение во время простоя: ") +
            QString::number(constant) + '\n';

    additional_info_text +=
            QString::fromUtf8("Амплитуда: ") +
            QString::number(amplitude) + '\n';

    additional_info_text +=
            QString::fromUtf8("Момент 50%-ной амплитуды на подъёме: ") +
            QString::number(time_up_50_percent) + '\n';
    additional_info_text +=
            QString::fromUtf8("Значение 50%-ной амплитуды на подъёме: ") +
            QString::number(approximation.getValue(time_up_50_percent)) + '\n';

    additional_info_text +=
            QString::fromUtf8("Момент 50%-ной амплитуды на спуске: ") +
            QString::number(time_down_50_percent) + '\n';
    additional_info_text +=
            QString::fromUtf8("Значение 50%-ной амплитуды на спуске: ") +
            QString::number(approximation.getValue(time_down_50_percent)) + '\n';

    double velocity_max_up_time = findMaximum([this](const double &key){return approximation.getFirstDerivativeValue(key);}, PRECISION, peak_time_1 - (peak_time_1 - start) / 2.0);

    double velocity_max_down_time = findMaximum([this](const double &key){return -approximation.getFirstDerivativeValue(key);}, PRECISION, peak_time_2 + (end - peak_time_2) / 2.0);

    additional_info_text +=
            QString::fromUtf8("Максимальная скорость на подъёме: ") +
            QString::number(approximation.getFirstDerivativeValue(velocity_max_up_time)) + '\n';
    additional_info_text +=
            QString::fromUtf8("Максимальная скорость на спуске: ") +
            QString::number(-approximation.getFirstDerivativeValue(velocity_max_down_time)) + '\n';

    additional_info_text +=
            QString::fromUtf8("Скорость на подъёме (тип 1): ") +
            QString::number(amplitude / (peak_time_1 - start)) + '\n';

    additional_info_text +=
            QString::fromUtf8("Скорость на спуске (тип 1): ") +
            QString::number(amplitude_2 / (end - peak_time_2)) + '\n';

    double time = end - start;

    additional_info_text +=
            QString::fromUtf8("Полное время: ") +
            QString::number(time) + '\n';

    double integral = findIntegral([this](const double &key) {return approximation.getValue(key);}, start, end, PRECISION);

    additional_info_text +=
            QString::fromUtf8("Интеграл: ") +
            QString::number(integral) + '\n';

    additional_info_text +=
            QString::fromUtf8("Скорость на подъёме (тип 2): ") +
            QString::number((approximation.getValue(time_up_90_percent) - approximation.getValue(time_up_10_percent)) / (time_up_90_percent - time_up_10_percent)) + '\n';

    additional_info_text +=
            QString::fromUtf8("Скорость на спуске (тип 2): ") +
            QString::number((approximation.getValue(time_down_90_percent) - approximation.getValue(time_down_10_percent)) / (time_down_10_percent - time_down_90_percent)) + '\n';


    result += "\n\n" + additional_info_text;
    file.write(result.toUtf8());
    file.close();
    QMessageBox msgBox;
    msgBox.setWindowTitle(QString::fromUtf8("Успех"));
    msgBox.setText(QString::fromUtf8("Результаты успешно сохранены!"));
    msgBox.exec();
}

void MainWindow::on_open_results_from_file_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Открыть результаты"), "", tr("Результаты (*.nresult);;Все файлы (*)"));
    QFile file(file_name);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString::fromUtf8("Ошибка"));
        msgBox.setText(QString::fromUtf8("Произошла ошибка при открытии файла!"));
        msgBox.exec();
        return;
    }
    QVector<double> result(7);
    unsigned int index = 0;
    bool number_read_status_ok = false;
    QString file_line = "", number = "";


    while (!file.atEnd() && file_line.left(5) != "a1 = ") {
        file_line = file.readLine();
    }
    if (file.atEnd()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString::fromUtf8("Ошибка"));
        msgBox.setText(QString::fromUtf8("Произошла ошибка при чтении данных из файла!"));
        msgBox.exec();
        return;
    }
    number = "";
    index = 5;
    while (index < (unsigned int)file_line.size() && (file_line[index].isDigit() || file_line[index] == '.' || file_line[index] == '-')) {
        number += file_line[index];
        index++;
    }
    result[0] = number.toDouble(&number_read_status_ok);
    if (!number_read_status_ok) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString::fromUtf8("Ошибка"));
        msgBox.setText(QString::fromUtf8("Произошла ошибка при чтении данных из файла!"));
        msgBox.exec();
        return;
    }
    number = "";
    index += 7;
    while (index < (unsigned int)file_line.size() && (file_line[index].isDigit() || file_line[index] == '.' || file_line[index] == '-')) {
        number += file_line[index];
        index++;
    }
    result[1] = number.toDouble(&number_read_status_ok);
    if (!number_read_status_ok) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString::fromUtf8("Ошибка"));
        msgBox.setText(QString::fromUtf8("Произошла ошибка при чтении данных из файла!"));
        msgBox.exec();
        return;
    }
    number = "";
    index += 7;
    while (index < (unsigned int)file_line.size() && (file_line[index].isDigit() || file_line[index] == '.' || file_line[index] == '-')) {
        number += file_line[index];
        index++;
    }
    result[2] = number.toDouble(&number_read_status_ok);
    if (!number_read_status_ok) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString::fromUtf8("Ошибка"));
        msgBox.setText(QString::fromUtf8("Произошла ошибка при чтении данных из файла!"));
        msgBox.exec();
        return;
    }


    while (!file.atEnd() && file_line.left(5) != "a2 = ") {
        file_line = file.readLine();
    }
    if (file.atEnd()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString::fromUtf8("Ошибка"));
        msgBox.setText(QString::fromUtf8("Произошла ошибка при чтении данных из файла!"));
        msgBox.exec();
        return;
    }
    number = "";
    index = 5;
    while (index < (unsigned int)file_line.size() && (file_line[index].isDigit() || file_line[index] == '.' || file_line[index] == '-')) {
        number += file_line[index];
        index++;
    }
    result[3] = number.toDouble(&number_read_status_ok);
    if (!number_read_status_ok) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString::fromUtf8("Ошибка"));
        msgBox.setText(QString::fromUtf8("Произошла ошибка при чтении данных из файла!"));
        msgBox.exec();
        return;
    }
    number = "";
    index += 7;
    while (index < (unsigned int)file_line.size() && (file_line[index].isDigit() || file_line[index] == '.' || file_line[index] == '-')) {
        number += file_line[index];
        index++;
    }
    result[4] = number.toDouble(&number_read_status_ok);
    if (!number_read_status_ok) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString::fromUtf8("Ошибка"));
        msgBox.setText(QString::fromUtf8("Произошла ошибка при чтении данных из файла!"));
        msgBox.exec();
        return;
    }
    number = "";
    index += 7;
    while (index < (unsigned int)file_line.size() && (file_line[index].isDigit() || file_line[index] == '.' || file_line[index] == '-')) {
        number += file_line[index];
        index++;
    }
    result[5] = number.toDouble(&number_read_status_ok);
    if (!number_read_status_ok) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString::fromUtf8("Ошибка"));
        msgBox.setText(QString::fromUtf8("Произошла ошибка при чтении данных из файла!"));
        msgBox.exec();
        return;
    }


    while (!file.atEnd() && file_line.left(8) != "const = ") {
        file_line = file.readLine();
    }
    if (file.atEnd()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString::fromUtf8("Ошибка"));
        msgBox.setText(QString::fromUtf8("Произошла ошибка при чтении данных из файла!"));
        msgBox.exec();
        return;
    }
    number = "";
    index = 8;
    while (index < (unsigned int)file_line.size() && (file_line[index].isDigit() || file_line[index] == '.' || file_line[index] == '-')) {
        number += file_line[index];
        index++;
    }
    result[6] = number.toDouble(&number_read_status_ok);
    if (!number_read_status_ok) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString::fromUtf8("Ошибка"));
        msgBox.setText(QString::fromUtf8("Произошла ошибка при чтении данных из файла!"));
        msgBox.exec();
        return;
    }

    results_windows.push_back(new FunctionApproximationTabWidget(nullptr, FunctionApproximation(result)));
    results_windows[results_windows.size() - 1]->show();
}

void MainWindow::on_analyze_start_clicked()
{
    ExperimentAnalyzer analyzer(std::unique_ptr<AbstractFunctionApproximator>(new NetFunctionApproximator()));
    ExperimentAnalysis analysis = analyzer.analyze(experiment);
    analysis_windows.push_back(new ExperimentAnalysisDialog(analysis));
    analysis_windows.last()->show();
}

