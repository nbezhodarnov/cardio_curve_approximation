#ifndef ABSTRACTFUNCTIONEXTRACTION_H
#define ABSTRACTFUNCTIONEXTRACTION_H

#include <QWidget>

#include "function.h"

class AbstractFunctionExtraction
{
public:
    AbstractFunctionExtraction(QWidget *ptr = nullptr);
    virtual ~AbstractFunctionExtraction();
    virtual Function extract() = 0;

protected:
    QWidget *parent;
};

#endif // ABSTRACTFUNCTIONEXTRACTION_H
