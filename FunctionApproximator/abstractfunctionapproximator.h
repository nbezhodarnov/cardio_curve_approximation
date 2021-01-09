#ifndef ABSTRACTFUNCTIONAPPROXIMATOR_H
#define ABSTRACTFUNCTIONAPPROXIMATOR_H

#include <QWidget>

#include "function.h"
#include "functionapproximation.h"

class AbstractFunctionApproximator
{
public:
    AbstractFunctionApproximator(QWidget *ptr = nullptr);
    virtual ~AbstractFunctionApproximator();
    virtual FunctionApproximation approximate(const Function&) = 0;

protected:
    QWidget *parent;
};

#endif // ABSTRACTFUNCTIONAPPROXIMATOR_H
