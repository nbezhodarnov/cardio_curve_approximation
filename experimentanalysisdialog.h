#ifndef EXPERIMENTANALYSISDIALOG_H
#define EXPERIMENTANALYSISDIALOG_H

#include <QDialog>

#include "ExperimentAnalyzer/experimentanalysis.h"

namespace Ui {
class ExperimentAnalysisDialog;
}

class ExperimentAnalysisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExperimentAnalysisDialog(ExperimentAnalysis newAnalysis, QWidget *parent = nullptr);
    ~ExperimentAnalysisDialog();

private slots:
    void on_exportPushButton_clicked();

private:
    Ui::ExperimentAnalysisDialog *ui;
    ExperimentAnalysis analysis;
};

#endif // EXPERIMENTANALYSISDIALOG_H
