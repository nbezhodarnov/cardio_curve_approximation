#ifndef FUNCTIONAPPROXIMATIONTABWIDGET_H
#define FUNCTIONAPPROXIMATIONTABWIDGET_H

#include <functional>
#include <cmath>

#include <QTabWidget>

#include "mathfunctions.h"
#include "functionapproximation.h"

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
