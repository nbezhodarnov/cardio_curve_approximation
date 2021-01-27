#include <QTextStream>

#include "ndatfilefunctionextraction.h"

NdatFileFunctionExtraction::NdatFileFunctionExtraction(QString fileName, QWidget *ptr): AbstractFunctionExtraction(ptr), file(fileName) {
}

NdatFileFunctionExtraction::~NdatFileFunctionExtraction() {
    file.close();
}

Function NdatFileFunctionExtraction::extract() {
    if (!file.exists() || (!file.isOpen() && !file.open(QIODevice::ReadOnly))) {
        QTextStream(stderr) << "Error! Couldn't open file.\n";
        return Function();
    }
    Function result;
    while (!file.atEnd()) {
        QString line = file.readLine();
        QStringList space = line.split(' ');
        bool first, second;
        double x = space[0].toDouble(&first);
        double f = space[1].toDouble(&second);
        if ((first == false) || (second == false)) {
            file.close();
            QTextStream(stderr) << "Error! Wrong file format (should be ndat file format).\n";
            return Function();
        }
        result.add({x, f});
    }
    file.seek(0);
    return result;
}
