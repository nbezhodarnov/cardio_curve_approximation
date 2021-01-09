#-------------------------------------------------
#
# Project created by QtCreator 2020-03-07T11:08:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Kurs_QTWidgets
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG     += debug


SOURCES += \
    FunctionApproximator/NetFunctionApproximator/netfunctionapproximator.cpp \
    FunctionApproximator/abstractfunctionapproximator.cpp \
    FunctionExtraction/OldFileFunctionExtraction/oldfilefunctionextraction.cpp \
    FunctionExtraction/OldFileFunctionExtraction/oldfilefunctionextractiondialog.cpp \
    FunctionExtraction/abstractfunctionextraction.cpp \
    FunctionExtraction/filefunctionextractionmanager.cpp \
    function.cpp \
    functionapproximation.cpp \
    functionelement.cpp \
        main.cpp \
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    FunctionApproximator/NetFunctionApproximator/netfunctionapproximator.h \
    FunctionApproximator/abstractfunctionapproximator.h \
    FunctionExtraction/OldFileFunctionExtraction/oldfilefunctionextraction.h \
    FunctionExtraction/OldFileFunctionExtraction/oldfilefunctionextractiondialog.h \
    FunctionExtraction/abstractfunctionextraction.h \
    FunctionExtraction/filefunctionextractionmanager.h \
    function.h \
    functionapproximation.h \
    functionelement.h \
        mainwindow.h \
    qcustomplot.h

FORMS += \
        FunctionExtraction/OldFileFunctionExtraction/oldfilefunctionextractiondialog.ui \
        mainwindow.ui
