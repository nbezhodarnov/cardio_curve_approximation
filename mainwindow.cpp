#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextStream>
#include <QFileDialog>
#include <QString>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    x.resize(256);
    f.resize(256);

    ui->setupUi(this);
    ui->widget->setVisible(false);

    start = nullptr;
    start_v = ui->widget->xAxis->range().upper;
    end = nullptr;
    end_v = ui->widget->xAxis->range().lower;
    end_turn = false;
    verticalLine = nullptr;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
        double ystart = ui->widget->yAxis->range().lower;
        double yend = ui->widget->yAxis->range().upper;
        double key = ui->widget->xAxis->pixelToCoord(event->pos().x());

        QVector<double> x(2), y(2);
        x[0] = x[1] = key;
        y[0] = ystart;
        y[1] = yend;

        verticalLine->setData(x, y);
        ui->widget->replot();
}

void MainWindow::mouseClickEvent(QMouseEvent *event)
{
        double ystart = ui->widget->yAxis->range().lower;
        double yend = ui->widget->yAxis->range().upper;
        double key = ui->widget->xAxis->pixelToCoord(event->pos().x());

        qint16 i;
        for (i = 0; i < 256; i++) {
            if (x[i] >= key) {
                break;
            }
        }
        if (i != 0) {
            if (abs(key - x[i]) > abs(x[i - 1] - key)) {
                i--;
            }
        }
        key = x[i];

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
            start_v = key;
            end_turn = true;
            ui->widget->replot();
            return;
        } else if (end == nullptr) {
            end = new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);
            ui->widget->addPlottable(end);
            end->setName("End");
            end->setPen(QPen(QBrush(QColor(Qt::red)), 1));
            end->setData(x, y);
            end_v = key;
            if (key < start_v) {
                start->setVisible(false);
            }
            end_turn = false;
        } else if (end_turn) {
            end->setData(x, y);
            end_v = key;
            if (key < start_v) {
                start->setVisible(false);
            }
            end_turn = false;
            end->setVisible(true);
        } else {
            start->setData(x, y);
            start_v = key;
            if (key > end_v) {
                end->setVisible(false);
            }
            end_turn = true;
            start->setVisible(true);
        }
        ui->widget->replot();

        if ((start->visible()) && (end->visible())) {
            ui->approximation_start->setEnabled(true);
        } else {
            ui->approximation_start->setEnabled(false);
        }
}

MainWindow::~MainWindow()
{
    if (verticalLine != nullptr) {
        delete verticalLine;
    }
    if (start != nullptr) {
        delete start;
    }
    if (end != nullptr) {
        delete end;
    }
    if (file.isOpen()) {
        file.close();
    }
    delete ui;
}

qint64 OpenFileError() {
    QMessageBox msgBox;
    msgBox.setText(QString::fromUtf8("Произошла ошибка при открытии файла."));
    msgBox.setInformativeText(QString::fromUtf8("Желаете открыть другой файл?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    return msgBox.exec();
}

void SetMaxValue(QSpinBox *spinbox, QFile &file, qint64 experiment) {
    qint64 file_size = file.size() - 1;
    qint8 input_number = 0, f[256];

    file.seek(257 + (experiment - 1) * 768);
    for (quint8 i = 0; i < 20; ++i) {
        file.read((char *) &f[0], 1);
        if ((f[0] != 63) && (f[0] != 0)) {
            break;
        }
    }
    if ((f[0] == 63) || (f[0] == 0) || ((file.atEnd()) && (file_size % 768 > 0))) {
        input_number = 1;
    } else {
        file.seek(513 + (experiment - 1) * 768);
        for (quint8 i = 0; i < 20; ++i) {
            file.read((char *) &f[0], 1);
            if ((f[0] != 63) && (f[0] != 0)) {
                break;
            }
        }
        if ((f[0] == 63) || (f[0] == 0)  || ((file.atEnd()) && (file_size % 768 > 0))) {
            input_number = 2;
        } else {
            input_number = 3;
        }
    }

    spinbox->setRange(1, input_number);
}

void MainWindow::on_open_file_clicked()
{
    QString temp = file.fileName();
    if (file.isOpen()) {
        file.close();
    }
    qint64 ret = QMessageBox::Yes, experiments_count = 0, file_size;
    while (ret == QMessageBox::Yes) {
        QString file_name = QFileDialog::getOpenFileName(this, QString::fromUtf8("Открыть файл"), QDir::currentPath(), QString::fromUtf8("Все файлы (*)"));
        //QTextStream(stdout) << file_name << '\n' << flush;
        if (file_name == "") {
            if (temp != "") {
                file.setFileName(temp);
                file.open(QIODevice::ReadOnly);
            }
            return;
        }
        file.setFileName(file_name);
        file_size = file.size() - 1;
        if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
            ret = OpenFileError();
            continue;
        } else {
            experiments_count = file_size / 768;
            if (experiments_count == 0) {
                file.close();
                ret = OpenFileError();
                continue;
            } else {
                ret = 0;
            }
        }
    }
    if (ret == QMessageBox::No) {
        if (temp != "") {
            file.setFileName(temp);
            file.open(QIODevice::ReadOnly);
        }
        return;
    }
    if (file.isOpen()) {
        if (file_size % 768 > 0) {
            experiments_count++;
        }
        ui->experiment_choose->setRange(1, experiments_count);
        //ui->experiment_choose->setVisible(true);
        ui->experiment_choose->setEnabled(true);
        ui->channel_choose->setEnabled(true);
        ui->make_plot->setEnabled(true);
        SetMaxValue(ui->channel_choose, file, ui->experiment_choose->value());
    }
}

void MainWindow::on_experiment_choose_valueChanged(int arg1)
{
    SetMaxValue(ui->channel_choose, file, arg1);
}

void MainWindow::on_make_plot_clicked()
{
    QVector<double> f_temp(256);
    double step = 3.857;

    file.seek(1 + (ui->experiment_choose->value() - 1) * 768 + (ui->channel_choose->value() - 1) * 256);

    char temp;
    quint8 min = 255, max = 0;
    for (quint32 i = 0; i < 256; i++) {
        x[i] = i * step;
        file.read(&temp, 1);
        if ((unsigned char)temp < min) {
            min = temp;
        }
        if ((unsigned char)temp > max) {
            max = temp;
        }
        f_temp[i] = (unsigned char)temp;
        f[i] = (unsigned char)temp;
    }
    //ui->widget->setInteractions(QCP::iSelectPlottables);

    ui->widget->clearGraphs();
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(x, f_temp);

    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");

    ui->widget->xAxis->setRange(x[0], x[255]);

    ui->widget->yAxis->setRange(min, max);

    if (verticalLine == nullptr) {
        ui->widget->setVisible(true);
        ui->widget->setEnabled(true);
        ui->widget->setMouseTracking(true);
        verticalLine = new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);
        ui->widget->addPlottable(verticalLine);
        verticalLine->setName("Vertical");
        verticalLine->setPen(QPen(QBrush(QColor(Qt::black)), 1));
        connect(ui->widget, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMoveEvent(QMouseEvent*)));
        connect(ui->widget, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mouseClickEvent(QMouseEvent*)));
    }

    ui->widget->replot();
}
