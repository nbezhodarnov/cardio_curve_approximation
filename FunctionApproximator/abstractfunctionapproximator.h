#ifndef ABSTRACTFUNCTIONAPPROXIMATOR_H
#define ABSTRACTFUNCTIONAPPROXIMATOR_H

#include <array>

#include <QWidget>

#include "function.h"
#include "functionapproximation.h"

class AbstractFunctionApproximator
{
public:
    AbstractFunctionApproximator(QWidget *ptr = nullptr);
    virtual ~AbstractFunctionApproximator();
    virtual FunctionApproximation approximate(const Function &function, const FunctionApproximation &startPoint = FunctionApproximation(0, 0, 0, 0, 0, 0), const std::array<double, 3> &firstComponent = {0, 0, 0}) = 0;

protected:
    QWidget *parent;
};

#endif // ABSTRACTFUNCTIONAPPROXIMATOR_H
