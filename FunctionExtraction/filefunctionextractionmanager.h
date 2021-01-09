#ifndef FILEFUNCTIONEXTRACTIONMANAGER_H
#define FILEFUNCTIONEXTRACTIONMANAGER_H

#include <QWidget>

#include "abstractfunctionextraction.h"

qint64 OpenFileError();

class FileFunctionExtractionManager : public AbstractFunctionExtraction
{
public:
    FileFunctionExtractionManager(QWidget *ptr = nullptr);
    ~FileFunctionExtractionManager();
    Function extract();

private:
    AbstractFunctionExtraction *extractor;
};

#endif // FILEFUNCTIONEXTRACTIONMANAGER_H
