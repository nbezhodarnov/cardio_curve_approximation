#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

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

private:
    Ui::MainWindow *ui;
    QFile file;
};

#endif // MAINWINDOW_H
