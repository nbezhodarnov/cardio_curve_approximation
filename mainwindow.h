#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

#include <qcustomplot.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_open_file_clicked();

    void on_experiment_choose_valueChanged(int arg1);

    void on_make_plot_clicked();

    void mouseMoveEvent(QMouseEvent *event);

    void mouseClickEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    QFile file;
    QCPCurve *verticalLine, *start, *end;
    bool end_turn;
    double start_v, end_v;
    QVector<double> x;
    QVector<quint8> f;
};

#endif // MAINWINDOW_H
