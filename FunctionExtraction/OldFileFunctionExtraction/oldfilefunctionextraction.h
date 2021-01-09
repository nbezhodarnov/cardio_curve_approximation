#ifndef OLDFILEFUNCTIONEXTRACTION_H
#define OLDFILEFUNCTIONEXTRACTION_H

#include <QFile>

#include "FunctionExtraction/abstractfunctionextraction.h"

class OldFileFunctionExtraction : public AbstractFunctionExtraction
{
public:
    OldFileFunctionExtraction(QString fileName = "", QWidget *ptr = nullptr);
    ~OldFileFunctionExtraction();
    Function extract();

private:
    QFile file;
};

#endif // OLDFILEFUNCTIONEXTRACTION_H
