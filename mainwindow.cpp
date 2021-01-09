#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QTextStream>
#include <QFileDialog>

#include "FunctionExtraction/filefunctionextractionmanager.h"
#include "FunctionApproximator/NetFunctionApproximator/netfunctionapproximator.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    approximation(0, 0, 0, 0, 0, 0, 0)
{
    ui->setupUi(this);
    ui->widget->setVisible(false);
    ui->widget->setEnabled(false);

    start = nullptr;
    start_index = 0;
    end = nullptr;
    end_index = 0;
    end_turn = false;
    verticalLine = nullptr;

    extractor = new FileFunctionExtractionManager(this);
    for (int i = 0; i < ui->extractor_choose->count(); i++) {
        ui->extractor_choose->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }

    approximator = new NetFunctionApproximator(this);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (verticalLine == nullptr) {
        return;
    }
    double ystart = min_y - 3;
    double yend = max_y + 3;
    double key = ui->widget->xAxis->pixelToCoord(event->pos().x());

    QVector<double> x(2), y(2);
    x[0] = x[1] = key;
    y[0] = ystart;
    y[1] = yend;

    verticalLine->setData(x, y);
    ui->widget->replot();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (verticalLine == nullptr) {
        return;
    }
    if (event->button() == Qt::LeftButton) {
        xAxis[0] = ui->widget->xAxis->range().lower;
        xAxis[1] = ui->widget->xAxis->range().upper;
        yAxis[0] = ui->widget->yAxis->range().lower;
        yAxis[1] = ui->widget->yAxis->range().upper;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (verticalLine == nullptr) {
        return;
    }
    if (event->button() == Qt::LeftButton) {
        if (xAxis[0] == ui->widget->xAxis->range().lower &&
                xAxis[1] == ui->widget->xAxis->range().upper &&
                yAxis[0] == ui->widget->yAxis->range().lower &&
                yAxis[1] == ui->widget->yAxis->range().upper) {
            mouseClickEvent(event);
        }
    }
}

void MainWindow::mouseClickEvent(QMouseEvent *event)
{
    if (verticalLine == nullptr) {
        return;
    }
    double ystart = min_y - 3;
    double yend = max_y + 3;
    double key = ui->widget->xAxis->pixelToCoord(event->pos().x());

    unsigned int i;
    for (i = 0; i < function.size(); i++) {
        if (function.getKey(i) >= key) {
            break;
        }
    }
    if (i != 0) {
        if (abs(key - function.getKey(i)) > abs(function.getKey(i - 1) - key)) {
            i--;
        }
    }
    key = function.getKey(i);

    QVector<double> x(2), y(2);
    x[0] = x[1] = key;
    y[0] = ystart;
    y[1] = yend;

    if (start == nullptr) {
        start = new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);
        ui->widget->addPlottable(start);
        start->setName("Start");
        start->setPen(QPen(QBrush(QColor(Qt::red)), 1));
        start->setData(x, y);
        start_index = i;
        end_turn = true;
        ui->widget->replot();
        return;
    } else if (end == nullptr) {
        end = new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);
        ui->widget->addPlottable(end);
        end->setName("End");
        end->setPen(QPen(QBrush(QColor(Qt::red)), 1));
        end->setData(x, y);
        end_index = i;
        if (key < function.getKey(start_index)) {
            start->setVisible(false);
        }
        end_turn = false;
    } else if (end_turn) {
        end->setData(x, y);
        end_index = i;
        if (key < function.getKey(start_index)) {
            start->setVisible(false);
        }
        end_turn = false;
        end->setVisible(true);
    } else {
        start->setData(x, y);
        start_index = i;
        if (key > function.getKey(end_index)) {
            end->setVisible(false);
        }
        end_turn = true;
        start->setVisible(true);
    }
    ui->widget->replot();

    ui->approximation_start->setEnabled((start->visible()) && (end->visible()));
}

void MainWindow::on_plotrender_xaxis_range_changed(const QCPRange &newRange)
{
    if (verticalLine == nullptr) {
        return;
    }
    double lower = newRange.lower, upper = newRange.upper;
    if (lower < function.getKey(0)) {
        lower = function.getKey(0);
    }
    if (upper > function.getKey(function.size() - 1)) {
        upper = function.getKey(function.size() - 1);
    }
    ui->widget->xAxis->setRange(lower, upper);
}

void MainWindow::on_plotrender_yaxis_range_changed(const QCPRange &newRange)
{
    if (verticalLine == nullptr) {
        return;
    }
    double lower = newRange.lower, upper = newRange.upper;
    if (lower < min_y - 3) {
        lower = min_y - 3;
    }
    if (upper > max_y + 3) {
        upper = max_y + 3;
    }
    ui->widget->yAxis->setRange(lower, upper);
}

MainWindow::~MainWindow()
{
    /*if (verticalLine != nullptr) {
        delete verticalLine;
    }
    if (start != nullptr) {
        delete start;
    }
    if (end != nullptr) {
        delete end;
    }*/
    delete ui;
    delete extractor;
    delete approximator;
}

void MainWindow::on_make_plot_clicked()
{
    function = extractor->extract();
    if (function == Function()) {
        return;
    }
    max_y = function.getValue(0);
    min_y = function.getValue(0);
    for (unsigned int i = 1; i < function.size(); i++) {
        if (function.getValue(i) < min_y) {
            min_y = function.getValue(i);
        }
        if (function.getValue(i) > max_y) {
            max_y = function.getValue(i);
        }
    }
    ui->widget->clearGraphs();
    ui->widget->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(function);

    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");

    ui->widget->xAxis->setRange(function.getKey(0), function.getKey(function.size() - 1));

    ui->widget->yAxis->setRange(min_y, max_y);

    if (verticalLine == nullptr) {
        ui->widget->setVisible(true);
        ui->widget->setEnabled(true);
        ui->widget->setMouseTracking(true);
        verticalLine = new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);
        ui->widget->addPlottable(verticalLine);
        verticalLine->setName("Vertical");
        verticalLine->setPen(QPen(QBrush(QColor(Qt::black)), 1));
        connect(ui->widget, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMoveEvent(QMouseEvent*)));
        connect(ui->widget, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
        connect(ui->widget, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseReleaseEvent(QMouseEvent*)));
        connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(on_plotrender_xaxis_range_changed(QCPRange)));
        connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(on_plotrender_yaxis_range_changed(QCPRange)));
    } else {
        bool has_been_cleaned = false;
        if (start != nullptr) {
            ui->widget->removePlottable(start);
            //delete start;
            start = nullptr;
            has_been_cleaned = true;
        }
        if (end != nullptr) {
            ui->widget->removePlottable(end);
            //delete end;
            end = nullptr;
            has_been_cleaned = true;
        }
        if (has_been_cleaned) {
            ui->approximation_start->setEnabled(false);
            start_index = 0;
            end_index = 0;
        }
    }

    ui->widget->replot();
}

void MainWindow::on_approximation_start_clicked()
{
    Function data;
    for (unsigned int i = start_index; i <= end_index; i++) {
        data.add(function.getElement(i));
    }
    approximation = approximator->approximate(data);

    unsigned int n = 100;
    QVector<double> x_result(n), result(n), e_first(n), e_second(n);
    double max = -INFINITY, min = INFINITY, step = (function.getKey(end_index) - function.getKey(start_index)) / (double)(n - 1);
    for (quint8 i = 0; i < n; i++) {
        x_result[i] = function.getKey(start_index) + i * step;
        e_first[i] = approximation.getFirstComponentValue(x_result[i]);
        e_second[i] = approximation.getSecondComponentValue(x_result[i]);
        result[i] = e_first[i] + e_second[i] + approximation.getCoefficients()[6];
        e_first[i] += approximation.getCoefficients()[6];
        e_second[i] += approximation.getCoefficients()[6];
        if (max < result[i]) {
            max = result[i];
        }
        if (min > result[i]) {
            min = result[i];
        }
        //out << x_result[i] << " - " << result[i] << '\n' << flush;
    }
    bool max_min_changed = false;
    if (max > max_y) {
        max_y = max;
        max_min_changed = true;
    }
    if (min < min_y) {
        min_y = min;
        max_min_changed = true;
    }
    if (max_min_changed) {
        if (this->start->visible()) {
            QVector<double> x_temp(2), y_temp(2);
            x_temp[0] = x_temp[1] = function.getKey(start_index);
            y_temp[0] = min_y - 3;
            y_temp[1] = max_y + 3;
            this->start->setData(x_temp, y_temp);
            ui->widget->replot();
        }
        if (this->end->visible()) {
            QVector<double> x_temp(2), y_temp(2);
            x_temp[0] = x_temp[1] = function.getKey(end_index);
            y_temp[0] = min_y - 3;
            y_temp[1] = max_y + 3;
            this->end->setData(x_temp, y_temp);
            ui->widget->replot();
        }
    }
    //out << coefficient << '\n' << flush;
    if (ui->widget->graphCount() <= 1) {
        ui->widget->addGraph();
        ui->widget->graph(ui->widget->graphCount() - 1)->setPen(QPen(QBrush(QColor(Qt::magenta)), 1));
        ui->widget->addGraph();
        ui->widget->graph(ui->widget->graphCount() - 1)->setPen(QPen(QBrush(QColor(Qt::darkYellow)), 1));
        ui->widget->addGraph();
        ui->widget->graph(ui->widget->graphCount() - 1)->setPen(QPen(QBrush(QColor(Qt::darkGreen)), 1));
    }
    ui->widget->graph(ui->widget->graphCount() - 3)->setData(x_result, result);
    ui->widget->graph(ui->widget->graphCount() - 2)->setData(x_result, e_first);
    ui->widget->graph(ui->widget->graphCount() - 1)->setData(x_result, e_second);
    ui->widget->replot();

    ui->results->setEnabled(true);
}

void MainWindow::on_results_clicked()
{
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
    case 0:
        extractor = new FileFunctionExtractionManager(this);
        break;
    default:
        extractor = new FileFunctionExtractionManager(this);
        break;
    }
}
