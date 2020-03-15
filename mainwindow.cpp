#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextStream>
#include <QFileDialog>
#include <QString>
#include <QFile>

#include <thread>
#include <sys/prctl.h>
#include <unordered_map>

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
    if (event->button() == Qt::LeftButton) {
            xAxis[0] = ui->widget->xAxis->range().lower;
            xAxis[1] = ui->widget->xAxis->range().upper;
            yAxis[0] = ui->widget->yAxis->range().lower;
            yAxis[1] = ui->widget->yAxis->range().upper;
        }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
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
        double ystart = min_y - 3;
        double yend = max_y + 3;
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

void MainWindow::on_plotrender_xaxis_range_changed(const QCPRange &newRange)
{
    double lower = newRange.lower, upper = newRange.upper;
    if (lower < x[0]) {
        lower = x[0];
    }
    if (upper > x[255]) {
        upper = x[255];
    }
    ui->widget->xAxis->setRange(lower, upper);
}

void MainWindow::on_plotrender_yaxis_range_changed(const QCPRange &newRange)
{
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
    msgBox.setWindowTitle(QString::fromUtf8("Ошибка"));
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
    min_y = min;
    max_y = max;
    if (ui->widget->graphCount() == 0) {
        ui->widget->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);

        ui->widget->clearGraphs();
        ui->widget->addGraph();
    }
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
        connect(ui->widget, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
        connect(ui->widget, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseReleaseEvent(QMouseEvent*)));
        connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(on_plotrender_xaxis_range_changed(QCPRange)));
        connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(on_plotrender_yaxis_range_changed(QCPRange)));
    }

    ui->widget->replot();
}

/*qint64 PleaseWaitWindow() {
    QMessageBox msgBox;
    msgBox.setText(QString::fromUtf8("Произошла ошибка при открытии файла."));
    msgBox.setInformativeText(QString::fromUtf8("Желаете открыть другой файл?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    return msgBox.exec();
}*/

QMessageBox* MainWindow::PleaseWaitWindow() {
    prctl(PR_SET_NAME,"pleaswaitwindow",0,0,0);
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setWindowTitle(QString::fromUtf8("Пожалуйста, подождите"));
    msgBox->setText(QString::fromUtf8("Пожалуйста, подождите. Идут вычисления."));
    msgBox->setStandardButtons(0);
    msgBox->exec();
    return msgBox;
}

void MainWindow::on_approximation_start_clicked()
{
    //QMessageBox *msgBox = PleaseWaitWindow();

    QTextStream out(stdout);
    quint8 start = 0, end;
    while (x[start] != start_v) {
        start++;
    }
    end = start;
    while (x[end] != end_v) {
        end++;
    }
    end++;
    qint16 min_local = start;
    for (int i = start + 1; i < end; ++i) {
        if (f[i] < f[min_local]) {
            min_local = i;
        }
    }
    qint16 coefficient = f[min_local];
    for (int i = start; i < end; ++i) {
        f[i] -= coefficient;
    }

    qint16 n = end - start;
    int index = start + n / 10, peak_beginning;
    bool is_peak_unavailable = false;
    double a1, a2, b1, b2, c1, c2, step = 3.857;
    while (f[index] <= f[index + 1]) {
        if (f[index] + coefficient == 128) {
            if (f[index - 1] + coefficient < 128) {
                peak_beginning = index;
                is_peak_unavailable = true;
            }
        }
        index++;
        if ((f[index + 1] + coefficient < 128) && (is_peak_unavailable)) {
            index = (peak_beginning + index) / 2;
            break;
        }
    }
    c1 = x[index];
    a1 = f[index];
    b1 = std::abs(-log(f[start + (index - start) * 100 / 245] / a1) / (pow(x[start + (index - start) * 100 / 245] - c1, 2)));

    if (is_peak_unavailable) {
        a1 += coefficient / 2;
    }

    index = int(start + n * 0.9);
    is_peak_unavailable = false;
    while (f[index] <= f[index - 1]) {
        if (f[index] + coefficient == 128) {
            if (f[index + 1] + coefficient < 128) {
                peak_beginning = index;
                is_peak_unavailable = true;
            }
        }
        index--;
        if ((f[index - 1] + coefficient < 128) && (is_peak_unavailable)) {
            index = (peak_beginning + index) / 2;
            break;
        }
    }
    c2 = x[index];
    a2 = f[index];
    b2 = std::abs(-log(f[index + (n - index) * 100 / 182] / a2) / (pow(x[index + (n - index) * 100 / 182] - c2, 2)));

    bool long_line = false;
    if ((c1 > c2) && (!is_peak_unavailable)) {
        long_line = true;
        int max_local = a1;
        index = start + n / 10;
        while ((max_local - f[index] > 4) || (f[index] < f[index + 1])) {
            index++;
        }
        c1 = x[index];

        index = int(start + n * 0.9);
        while ((max_local - f[index] > 4) || (f[index] < f[index - 1])) {
            index--;
        }
        c2 = x[index];
    }

    if (is_peak_unavailable) {
        a2 += coefficient / 2;
    }

    bool is_two_extremums = true;
    if (std::abs(c1 - c2) <= step) {
        int index_temp = start + n;
        while (f[index_temp - 1] < f[index_temp]) {
            index_temp--;
        }
        while ((f[index_temp] <= f[index_temp - 1]) && (index_temp > int(n * 0.9))) {
            index_temp--;
        }
        if (index_temp <= n * 0.9) {
            index_temp = start;
            while(f[index_temp + 1] < f[index_temp]) {
                index_temp++;
            }
            while ((f[index_temp] <= f[index_temp + 1]) && (index_temp < n / 10)) {
                index_temp++;
            }
            if (index_temp >= 100) {
                is_two_extremums = false;
            } else {
                c1 = x[index_temp];
                a1 = f[index_temp];
                b1 = std::abs(-log(f[start + (index_temp - start) / 2] / a1) / (pow(x[start + (index_temp - start) / 2] - c1, 2)));
            }
        } else {
            c2 = x[index_temp];
            a2 = f[index_temp];
            b2 = std::abs(-log(f[index_temp + (n - index_temp) / 2] / a2) / (pow(x[index_temp + (n - index_temp) / 2] - c2, 2)));
        }
    }


    out << "a1 = " << a1 << ", b1 = " << b1 << ", c1 = " << c1 << '\n' << flush;
    if (is_two_extremums) {
        out << "a2 = " << a2 << ", b2 = " << b2 << ", c2 = " << c2 << "\n\n" << flush;
    } else {
        a1 /= 2;
        a2 /= 2;
        out << '\n';
    }

    double step1 = 0.5, step2 = 0.01, step3 = 0.1, min_value = INFINITY, previous_value, temp;
    if ((a1 > 1000) || (a2 > 1000)) {
        step1 = 2;
        step3 = 0.05;
    } else if ((a1 > 100) || (a2 > 100)) {
        step1 = 1;
        step3 = 0.1;
    } else if ((a1 > 10) || (a2 > 10)) {
        step1 = 0.5;
        step3 = 0.5;
    } else {
        step1 = 0.1;
        step3 = 1;
    }
    if ((b1 > 10) || (b2 > 10)) {
        step2 = 0.5;
    } else if ((b1 > 1) || (b2 > 1)) {
        step2 = 0.05;
    } else {
        step2 = 0.01;
    }
    if (!is_two_extremums) {
        step1 *= 2;
        step2 *= 2;
        step3 *= 2;
    }
    int numbers_of_points = n, counter = 0;
    if (n < numbers_of_points) {
        numbers_of_points = n;
    }
    if ((n > 150) && (!long_line)) {
        numbers_of_points = 50;
    } else if (n > 150) {
        numbers_of_points = 100;
    }
    uint8_t min_point_indexes[6];
    for (uint8_t i = 0; i < 6; ++i) {
        min_point_indexes[i] = 0;
    }
    bool q = true, need_center_correction = false;
    uint8_t center_correcter = 1;
    int little_changes_count = 0;
    long double reset[6];
    while (min_value > 18) {
    q = false;
    previous_value = min_value;
    for (uint8_t i = 0; i < 5; ++i) {
        for (uint8_t j = 0; j < 5; ++j) {
            for (uint8_t k = 0; k < 5; ++k) {
                for (uint8_t l = 0; l < 5; ++l) {
                    for (uint8_t m = 0; m < 5; ++m) {
                        for (uint8_t r = 0; r < 5; ++r) {
                            temp = 0;
                            for (int p = 0; p < numbers_of_points; ++p) {
                                center_correcter = 1;
                                if ((p > n * 0.25) && (p < n * 0.75)) {
                                    if (need_center_correction) {
                                        center_correcter = 8;
                                    }
                                }
                                if (f[start + p * n / numbers_of_points] + coefficient < 128) {
                                    temp += center_correcter * std::abs(f[start + p * (n / numbers_of_points)] - std::abs(a1 - (2 - i) * step1) * exp(-std::abs(b1 - (2 - j) * step2) * pow(x[start + p * (n / numbers_of_points)] - c1 + (2 - k) * step3, 2)) - std::abs(a2 - (2 - l) * step1) * exp(-std::abs(b2 - (2 - m) * step2) * pow(x[start + p * (n / numbers_of_points)] - c2 + (2 - r) * step3, 2)));
                                }
                            }
                            if (temp < min_value) {
                                q = true;
                                min_value = temp;
                                min_point_indexes[0] = i;
                                min_point_indexes[1] = j;
                                min_point_indexes[2] = k;
                                min_point_indexes[3] = l;
                                min_point_indexes[4] = m;
                                min_point_indexes[5] = r;
                            }
                        }
                    }
                }
            }
        }
    }
    if (!q) {
        step1 /= 10;
        step2 /= 10;
        step3 /= 10;
        if (step1 < 1e-20) {
            step1 = 0.5;
            step2 = 0.01;
            step3 = 0.1;

            if (((min_value > 160) && (!long_line)) || ((long_line) && (min_value > 200))) {
                temp = min_value + 301;
                reset[0] = a1;
                reset[1] = b1;
                reset[2] = c1;
                reset[3] = a2;
                reset[4] = b2;
                reset[5] = c2;
                while (temp - min_value > 300) {
                    a1 = reset[0] + (rand() % 100 - 50) / 1.0;
                    b1 = reset[1] + (rand() % 10 - 5) / 10.0;
                    c1 = reset[2] + (rand() % 10 - 5) / 1.0;
                    a2 = reset[3] + (rand() % 100 - 50) / 1.0;
                    b2 = reset[4] + (rand() % 10 - 5) / 10.0;
                    c2 = reset[5] + (rand() % 10 - 5) / 1.0;

                    temp = 0;
                    for (int p = 0; p < numbers_of_points; ++p) {
                        center_correcter = 1;
                        if ((p > n * 0.25) && (p < n * 0.75)) {
                            if (need_center_correction) {
                                center_correcter = 8;
                            }
                        }
                        if (f[start + p * n / numbers_of_points] + coefficient < 128) {
                            temp += center_correcter * std::abs(f[start + p * (n / numbers_of_points)] - std::abs(a1) * exp(-std::abs(b1) * pow(x[start + p * (n / numbers_of_points)] - c1, 2)) - std::abs(a2) * exp(-std::abs(b2) * pow(x[start + p * (n / numbers_of_points)] - c2, 2)));
                        }
                    }
                }
            }

            min_value = temp;
            counter = 0;
        }
        continue;
    }
    a1 -= (2 - min_point_indexes[0]) * step1;
    b1 -= (2 - min_point_indexes[1]) * step2;
    c1 -= (2 - min_point_indexes[2]) * step3;
    a2 -= (2 - min_point_indexes[3]) * step1;
    b2 -= (2 - min_point_indexes[4]) * step2;
    c2 -= (2 - min_point_indexes[5]) * step3;
    if ((std::abs(previous_value - min_value) < 0.0001) && (((min_value < 160) && (!long_line)) || ((long_line) && (min_value < 200)))) {
        little_changes_count++;
        if (little_changes_count >= 10) {
            break;
        }
    } else {
        little_changes_count = 0;
    }
    if ((min_point_indexes[0] % 4 != 0) && (min_point_indexes[1] % 4 != 0) && (min_point_indexes[2] % 4 != 0) && (min_point_indexes[3] % 4 != 0) && (min_point_indexes[4] % 4 != 0) && (min_point_indexes[5] % 4 != 0)) {
        step1 /= 10;
        step2 /= 10;
        step3 /= 10;
        //counter = 0;
    } else {
        counter++;
        if ((counter % 10 == 0) && (step2 < 0.01)) {
            step1 = 0.5 * (counter / 10);
            step2 = 0.01 * (counter / 10);
            step3 = 0.1 * (counter / 10);
            //counter = 0;
        }
        if ((counter % 100 == 0) && (min_value > 160)) {
            step1 = 0.5;
            step2 = 0.01;
            step3 = 0.1;

            temp = min_value + 301;
            reset[0] = a1;
            reset[1] = b1;
            reset[2] = c1;
            reset[3] = a2;
            reset[4] = b2;
            reset[5] = c2;
            while (temp - min_value > 300) {
                a1 = reset[0] + (rand() % 100 - 50) / 1.0;
                b1 = reset[1] + (rand() % 10 - 5) / 10.0;
                c1 = reset[2] + (rand() % 10 - 5) / 1.0;
                a2 = reset[3] + (rand() % 100 - 50) / 1.0;
                b2 = reset[4] + (rand() % 10 - 5) / 10.0;
                c2 = reset[5] + (rand() % 10 - 5) / 1.0;

                temp = 0;
                for (int p = 0; p < numbers_of_points; ++p) {
                    center_correcter = 1;
                    if ((p > n * 0.25) && (p < n * 0.75)) {
                        if (need_center_correction) {
                            center_correcter = 8;
                        }
                    }
                    if (f[start + p * n / numbers_of_points] + coefficient < 128) {
                        temp += center_correcter * std::abs(f[start + p * (n / numbers_of_points)] - std::abs(a1) * exp(-std::abs(b1) * pow(x[start + p * (n / numbers_of_points)] - c1, 2)) - std::abs(a2) * exp(-std::abs(b2) * pow(x[start + p * (n / numbers_of_points)] - c2, 2)));
                    }
                }
            }

            min_value = temp;
            counter = 0;
        }
    }
    temp = 0;
    if (!need_center_correction) {
        for (int p = 0; p < numbers_of_points; ++p) {
            center_correcter = 0;
            if ((p > n * 0.25) && (p < n * 0.75)) {
                center_correcter = 1;
            }
            if (f[start + p * n / numbers_of_points] + coefficient < 128) {
                temp += center_correcter * std::abs(f[start + p * (n / numbers_of_points)] - std::abs(a1) * exp(-std::abs(b1) * pow(x[start + p * (n / numbers_of_points)] - c1, 2)) - std::abs(a2) * exp(-std::abs(b2) * pow(x[start + p * (n / numbers_of_points)] - c2, 2)));
            }
        }
        if ((min_value < 80) && (temp > 50)) {
            need_center_correction = true;
            min_value = INFINITY;
        }
    }
    out << "a1 = " << a1 << ", b1 = " << b1 << ", c1 = " << c1 << '\n' << flush;
    out << "a2 = " << a2 << ", b2 = " << b2 << ", c2 = " << c2 << '\n' << flush;
    out << min_value << ' ' << previous_value << '\n' << flush;

    for (uint8_t i = 0; i < 6; ++i) {
        out << (int)min_point_indexes[i] << ' ' << flush;
    }
    out << '\n' << step1 << ' ' << step2 << ' ' << step3 << '\n' << flush;
    out << "\n\n" << flush;
    }

    a1 = std::abs(a1);
    a2 = std::abs(a2);
    b1 = std::abs(b1);
    b2 = std::abs(b2);


    out << "a1 = " << a1 << ", b1 = " << b1 << ", c1 = " << c1 << '\n' << flush;
    out << "a2 = " << a2 << ", b2 = " << b2 << ", c2 = " << c2 << '\n' << flush;

    QVector<double> x_result(n), result(n), e_first(n), e_second(n);
    double max = 0;
    for (quint8 i = 0; i < n; i++) {
        x_result[i] = x[i + start];
        result[i] = coefficient + a1 * exp(-b1 * pow(x_result[i] - c1, 2)) + a2 * exp(-b2 * pow(x_result[i] - c2, 2));
        e_first[i] = coefficient + a1 * exp(-b1 * pow(x_result[i] - c1, 2));
        e_second[i] = coefficient + a2 * exp(-b2 * pow(x_result[i] - c2, 2));
        if (max < result[i]) {
            max = result[i];
        }
        //out << x_result[i] << " - " << result[i] << '\n' << flush;
    }
    if (max > max_y) {
        max_y = max;
        if (this->start->visible()) {
            QVector<double> x_temp(2), y_temp(2);
            x_temp[0] = x_temp[1] = this->start_v;
            y_temp[0] = min_y - 3;
            y_temp[1] = max_y + 3;
            this->start->setData(x_temp, y_temp);
            ui->widget->replot();
        }
        if (this->end->visible()) {
            QVector<double> x_temp(2), y_temp(2);
            x_temp[0] = x_temp[1] = this->end_v;
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

    for (int i = start; i < end; ++i) {
        f[i] += coefficient;
    }

    //delete msgBox;
}
