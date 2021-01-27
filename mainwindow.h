#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

#include <qcustomplot.h>

#include "function.h"
#include "functionapproximation.h"
#include "FunctionExtraction/abstractfunctionextraction.h"
#include "FunctionApproximator/abstractfunctionapproximator.h"

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
    void on_make_plot_clicked();

    void mouseMoveEvent(QMouseEvent *event);

    void mouseClickEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void on_approximation_start_clicked();

    void plotrender_xaxis_range_changed(const QCPRange &);

    void plotrender_yaxis_range_changed(const QCPRange &);

    void on_results_clicked();

    void on_extractor_choose_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    QCPCurve *verticalLine, *start, *end;
    bool end_turn;
    double min_y, max_y, xAxis[2], yAxis[2];
    unsigned int start_index, end_index;
    Function function;
    FunctionApproximation approximation;
    AbstractFunctionExtraction *extractor;
    AbstractFunctionApproximator *approximator;
};

#endif // MAINWINDOW_H
