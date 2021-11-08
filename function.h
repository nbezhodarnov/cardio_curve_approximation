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
    void move(double, double);
    void moveByAxisX(double);
    void moveByAxisY(double);
    void removeElementAt(unsigned int);
    void removeElementsFromBeginning(unsigned int);
    void removeElementsFromEnd(unsigned int);
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
