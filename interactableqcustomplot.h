#ifndef INTERACTABLEQCUSTOMPLOT_H
#define INTERACTABLEQCUSTOMPLOT_H

#include "qcustomplot.h"
#include "function.h"

#include <QWidget>

namespace Ui {
class InteractableQCustomPlot;
}

class InteractableQCustomPlot : public QWidget
{
    Q_OBJECT

public:
    explicit InteractableQCustomPlot(QWidget *parent = nullptr, const Function& function_input = Function());
    ~InteractableQCustomPlot();

    virtual void setFunction(const Function&);

    double getMinimum();

    double getMaximum();

public slots:
    virtual void setVisible(bool visible);

private slots:
    void plotrender_xaxis_range_changed(QCPRange);

    void plotrender_yaxis_range_changed(QCPRange);

protected:
    Ui::InteractableQCustomPlot *ui;
    double min_y, max_y;
    Function function;
};

#endif // INTERACTABLEQCUSTOMPLOT_H
