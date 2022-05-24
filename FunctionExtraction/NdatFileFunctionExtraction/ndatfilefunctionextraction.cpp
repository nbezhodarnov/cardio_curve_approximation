#include <QTextStream>

#include "ndatfilefunctionextraction.h"

// Вычисление результата скользящего среднего
Function getMA(Function function, int size) {
    Function result;
    // size - количество отсчетов интервала усреднения
    double sumx = 0; // сумма отсчетов на интервале
    double *mas = new double[size]; // массив для хранения size отсчетов
    int index = 0; // индекс элемента массива
    for (int i = 0; i < size; i++) mas[i] = 0;
    for (unsigned int i = 0; i < function.size(); i++) {
        sumx -= mas[index];
        mas[index] = function.getValue(i);
        sumx += mas[index];
        index++;
        if (index >= size)
            index = 0; // возврат к началу "окна"
        result.add({function.getKey(i), sumx / size});
    }
    delete []mas;
    return result;
}

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
    result = getMA(result, 15);
    result.removeElementsFromBeginning(15);
    return result;
}
