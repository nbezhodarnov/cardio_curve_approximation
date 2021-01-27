#ifndef NETFUNCTIONAPPROXIMATOR_H
#define NETFUNCTIONAPPROXIMATOR_H

#include "FunctionApproximator/abstractfunctionapproximator.h"

struct LineIndexes {
    unsigned int begin;
    unsigned int end;

    LineIndexes(unsigned int begin_input = ~0, unsigned int end_input = 0);
    bool isIndexInLine(unsigned int);
    bool isIndexOutOfLine(unsigned int);
};

class NetFunctionApproximator : public AbstractFunctionApproximator
{
public:
    NetFunctionApproximator(QWidget *ptr = nullptr);
    FunctionApproximation approximate(const Function&);
};

#endif // NETFUNCTIONAPPROXIMATOR_H
