#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include "oldfilefunctionextraction.h"
#include "oldfilefunctionextractiondialog.h"

OldFileFunctionExtraction::OldFileFunctionExtraction(QString fileName, QWidget *ptr): AbstractFunctionExtraction(ptr), file(fileName) {
}

OldFileFunctionExtraction::~OldFileFunctionExtraction() {
    file.close();
}

Function OldFileFunctionExtraction::extract() {
    if (!file.exists() || (!file.isOpen() && !file.open(QIODevice::ReadOnly))) {
        QTextStream(stderr) << "Error! Couldn't open file.\n";
        return Function();
    }
    qint64 file_size = file.size() - 1, experiments_count = file_size / 768;
    if ((experiments_count == 0) || (file_size % 768 != 0)) {
        file.close();
        QTextStream(stderr) << "Error! Wrong file format (should be old file format).\n";
        return Function();
    }
    if (file_size % 768 > 0) {
        experiments_count++;
    }
    OldFileFunctionExtractionDialog dialogWindow(&file, parent);
    dialogWindow.ChangeExperimentsCount(experiments_count);
    if (dialogWindow.exec() == QMessageBox::Ok) {
        Function result;
        const double step = 3.857;

        file.seek(1 + (dialogWindow.GetExperimentNumber() - 1) * 768 + (dialogWindow.GetChannelNumber() - 1) * 256);

        char temp;
        for (quint32 i = 0; i < 256; i++) {
            file.read(&temp, 1);
            result.add({i * step, (double)(unsigned char)temp});
        }
        return result;
    }
    return Function();
}
