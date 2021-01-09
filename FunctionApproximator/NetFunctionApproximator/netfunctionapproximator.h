#ifndef NETFUNCTIONAPPROXIMATOR_H
#define NETFUNCTIONAPPROXIMATOR_H

#include "FunctionApproximator/abstractfunctionapproximator.h"

class NetFunctionApproximator : public AbstractFunctionApproximator
{
public:
    NetFunctionApproximator(QWidget *ptr = nullptr);
    FunctionApproximation approximate(const Function&);
};

#endif // NETFUNCTIONAPPROXIMATOR_H
