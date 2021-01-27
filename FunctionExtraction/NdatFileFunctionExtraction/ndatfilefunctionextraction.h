#ifndef NDATFILEFUNCTIONEXTRACTION_H
#define NDATFILEFUNCTIONEXTRACTION_H

#include <QFile>

#include "FunctionExtraction/abstractfunctionextraction.h"

class NdatFileFunctionExtraction : public AbstractFunctionExtraction
{
public:
    NdatFileFunctionExtraction(QString, QWidget *);
    ~NdatFileFunctionExtraction();
    Function extract();

private:
    QFile file;
};

#endif // NDATFILEFUNCTIONEXTRACTION_H
