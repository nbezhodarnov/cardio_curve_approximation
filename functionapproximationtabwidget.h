#ifndef FUNCTIONAPPROXIMATIONTABWIDGET_H
#define FUNCTIONAPPROXIMATIONTABWIDGET_H

#include <functional>
#include <cmath>

#include <QTabWidget>

#include "functionapproximation.h"

double findKeyByValue(const std::function<double (const double&)> &function, const double &value, const double &precision = 0.001, const double &start = 0);

double findMaximum(const std::function<double (const double&)> &function, const double &precision = 0.001, const double &start = 0);

double findIntegral(const std::function<double (const double&)> &function, const double &start, const double &end, const double &precision = 0.001);

namespace Ui {
class FunctionApproximationTabWidget;
}

class FunctionApproximationTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit FunctionApproximationTabWidget(QWidget*, const FunctionApproximation&);
    ~FunctionApproximationTabWidget();

private:
    Ui::FunctionApproximationTabWidget *ui;
};

#endif // FUNCTIONAPPROXIMATIONTABWIDGET_H
