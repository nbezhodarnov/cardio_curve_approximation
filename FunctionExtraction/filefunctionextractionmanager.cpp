#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include "filefunctionextractionmanager.h"
#include "OldFileFunctionExtraction/oldfilefunctionextraction.h"
#include "NdatFileFunctionExtraction/ndatfilefunctionextraction.h"

qint64 OpenFileError() {
    QMessageBox msgBox;
    msgBox.setWindowTitle(QString::fromUtf8("Ошибка"));
    msgBox.setText(QString::fromUtf8("Произошла ошибка при открытии файла."));
    msgBox.setInformativeText(QString::fromUtf8("Желаете открыть другой файл?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    return msgBox.exec();
}

FileFunctionExtractionManager::FileFunctionExtractionManager(QWidget *ptr): AbstractFunctionExtraction(ptr), extractor(nullptr) {
}

FileFunctionExtractionManager::~FileFunctionExtractionManager() {
    delete extractor;
}

Function FileFunctionExtractionManager::extract() {
    Function result;
    if (extractor != nullptr) {
        QMessageBox msgBox(QMessageBox::Icon(), "Открыть другой файл?", "Желаете открыть другой файл?", QMessageBox::Yes | QMessageBox::No, parent);
        msgBox.setDefaultButton(QMessageBox::Yes);
        if (msgBox.exec() != QMessageBox::Yes) {
            result = extractor->extract();
            if (result == Function()) {
                delete extractor;
                extractor = nullptr;
            } else {
                return result;
            }
        } else {
            delete extractor;
            extractor = nullptr;
        }
    }
    QFile file;
    qint64 ret = QMessageBox::Yes;
    while (ret == QMessageBox::Yes) {
        QString file_name = QFileDialog::getOpenFileName(parent, QString::fromUtf8("Открыть файл"), "", QString::fromUtf8("Файлы ndat (*.ndat);;Все файлы (*)"));
        if (file_name == "") {
            QTextStream(stdout) << "No file has been opened.\n" << flush;
            return Function();
        }
        file.setFileName(file_name);
        if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
            ret = OpenFileError();
            continue;
        } else {
            file.close();
            if (QFileInfo(file_name).suffix() == "ndat") {
                extractor = new NdatFileFunctionExtraction(file_name, parent);
                result = extractor->extract();
            } else {
                extractor = new OldFileFunctionExtraction(file_name, parent);
                result = extractor->extract();
            }

            if (result == Function()) {
                delete extractor;
                extractor = nullptr;
                ret = OpenFileError();
                continue;
            } else {
                return result;
            }
        }
    }
    return Function();
}
