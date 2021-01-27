#ifndef FUNCTION_H
#define FUNCTION_H

#include <QVector>

#include "functionelement.h"

class Function
{
public:
    Function();
    void add(FunctionElement);
    void add(double, double);
    FunctionElement getElement(unsigned int) const;
    FunctionElement getElement(int) const;
    double getKey(unsigned int) const;
    double getKey(int) const;
    double getValue(unsigned int) const;
    double getValue(int) const;
    double getValue(double) const;
    double getStep() const;
    unsigned int size() const;
    bool operator==(const Function&) const;
    bool operator!=(const Function&) const;

private:
    QVector<FunctionElement> elements;
};

#endif // FUNCTION_H
