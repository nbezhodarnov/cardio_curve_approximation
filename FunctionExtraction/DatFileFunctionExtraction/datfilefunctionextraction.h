#ifndef DATFILEFUNCTIONEXTRACTION_H
#define DATFILEFUNCTIONEXTRACTION_H

#include <QFile>

#include "FunctionExtraction/abstractfunctionextraction.h"

class DatFileFunctionExtraction : public AbstractFunctionExtraction
{
public:
    DatFileFunctionExtraction(QString, QWidget *);
    ~DatFileFunctionExtraction();
    Function extract();

private:
    QFile file;
};

#endif // NDATFILEFUNCTIONEXTRACTION_H
