#ifndef OLDFILEFUNCTIONEXTRACTIONDIALOG_H
#define OLDFILEFUNCTIONEXTRACTIONDIALOG_H

#include <QFile>
#include <QDialog>

namespace Ui {
class OldFileFunctionExtractionDialog;
}

class OldFileFunctionExtractionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit OldFileFunctionExtractionDialog(QFile*, QWidget *parent = nullptr);
    ~OldFileFunctionExtractionDialog();
    void ChangeExperimentsCount(qint64);
    void ChangeChannelsCount(qint64);
    qint64 GetExperimentNumber();
    qint64 GetChannelNumber();

signals:

private slots:
    void on_make_func_clicked();

    void on_experiment_choose_valueChanged(int);

private:
    Ui::OldFileFunctionExtractionDialog *ui;
    QFile *file;
};

#endif // OLDFILEFUNCTIONEXTRACTIONDIALOG_H
