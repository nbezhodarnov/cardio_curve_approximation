#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

#include <qcustomplot.h>

#include "function.h"
#include "functionapproximation.h"
#include "experimentanalysisdialog.h"
#include "functionapproximationtabwidget.h"
#include "ExperimentAnalyzer/experimentanalyzer.h"
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

    void on_rangeSelectionStateChanged(bool);

    void on_approximation_start_clicked();

    void on_results_clicked();

    void on_extractor_choose_currentIndexChanged(int index);

    void on_save_results_to_file_clicked();

    void on_open_results_from_file_clicked();

    void on_analyze_start_clicked();

private:
    Ui::MainWindow *ui;
    FunctionApproximation approximation;
    AbstractFunctionExtraction *extractor;
    AbstractFunctionApproximator *approximator;

    Function experiment;

    QVector<FunctionApproximationTabWidget*> results_windows;
    QVector<ExperimentAnalysisDialog*> analysis_windows;
};

#endif // MAINWINDOW_H
