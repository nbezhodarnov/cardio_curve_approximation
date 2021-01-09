#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include "filefunctionextractionmanager.h"
#include "OldFileFunctionExtraction/oldfilefunctionextraction.h"

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
        result = extractor->extract();
        if (result == Function()) {
            delete extractor;
            extractor = nullptr;
        } else {
            return result;
        }
    }
    QFile file;
    qint64 ret = QMessageBox::Yes;
    while (ret == QMessageBox::Yes) {
        QString file_name = QFileDialog::getOpenFileName(parent, QString::fromUtf8("Открыть файл"), QDir::currentPath(), QString::fromUtf8("Файлы dat (*.dat);;Все файлы (*)"));
        if (file_name == "") {
            QTextStream(stdout) << "No file has been opened.\n" << Qt::flush;
            return Function();
        }
        file.setFileName(file_name);
        if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
            ret = OpenFileError();
            continue;
        } else {
            file.close();
            extractor = new OldFileFunctionExtraction(file_name, parent);
            result = extractor->extract();

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
