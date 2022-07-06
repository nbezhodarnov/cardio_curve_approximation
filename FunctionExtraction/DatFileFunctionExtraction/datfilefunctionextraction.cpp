#include <QTextStream>

#include "mathfunctions.h"
#include "datfilefunctionextraction.h"

DatFileFunctionExtraction::DatFileFunctionExtraction(QString fileName, QWidget *ptr): AbstractFunctionExtraction(ptr), file(fileName) {
}

DatFileFunctionExtraction::~DatFileFunctionExtraction() {
    file.close();
}

Function DatFileFunctionExtraction::extract() {
    if (!file.exists() || (!file.isOpen() && !file.open(QIODevice::ReadOnly))) {
        QTextStream(stderr) << "Error! Couldn't open file.\n";
        return Function();
    }
    Function result;
    uint8_t channel = 0;
    double step = 1;
    double previousX = 0;
    std::vector<std::pair<double, double>> elements;
    while (!file.atEnd()) {
        QString line = file.readLine();
        QStringList parameters = line.split(',');
        bool isXValid, isFValid;
        double x = parameters[0].toDouble(&isXValid);
        double f = parameters[1 + channel * 3].toDouble(&isFValid);
        f *= 0.0425;
        f += 0.5;
        if ((isXValid == false) || (isFValid == false)) {
            file.close();
            QTextStream(stderr) << "Error! Wrong file format (should be dat file format).\n";
            return Function();
        }

        if (std::abs(x - previousX) > 0.00001) {
            step = (x - previousX) / elements.size();
            int i = 0;
            for (const auto &element : elements) {
                result.add({element.first + step * i, element.second});
                i++;
            }
            elements.clear();
        }
        previousX = x;
        elements.push_back({x, f});
    }
    int i = 0;
    for (const auto &element : elements) {
        result.add({element.first + step * i, element.second});
        i++;
    }
    elements.clear();
    file.seek(0);
    result = getMA(result, 10);
    result.removeElementsFromBeginning(10);
    return result;
}
