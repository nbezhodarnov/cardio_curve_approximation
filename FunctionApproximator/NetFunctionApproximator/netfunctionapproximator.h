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
    FunctionApproximation approximate(const Function &function, const std::array<double, 3> &firstComponent = {0, 0, 0});

private:
    double findConstant(const Function &function) const;
    Function normalizeFunction(const Function &function, const double &coefficient) const;
    FunctionApproximation calculateStartPoint(const Function &function, const std::array<double, 3> &firstComponent = {0, 0, 0}) const;
    FunctionApproximation calculateApproximation(const Function &function, const std::array<double, 3> &firstComponent = {0, 0, 0}) const;
};

#endif // NETFUNCTIONAPPROXIMATOR_H
