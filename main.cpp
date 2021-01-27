#include <QLocale>
#include <QString>
#include <QTranslator>
#include <QApplication>
#include <QLibraryInfo>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString translatorName = "qt_";
    translatorName += QLocale::system().name();
    QTranslator translator;
    if (translator.load(translatorName, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        a.installTranslator(&translator);
    }
    MainWindow w;
    w.show();
    return a.exec();
}
