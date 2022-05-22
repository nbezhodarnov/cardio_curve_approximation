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

SOURCES += \
    ExperimentAnalyzer/experimentanalysis.cpp \
    ExperimentAnalyzer/experimentanalyzer.cpp \
    FunctionApproximator/NetFunctionApproximator/netfunctionapproximator.cpp \
    FunctionApproximator/abstractfunctionapproximator.cpp \
    FunctionExtraction/DatFileFunctionExtraction/datfilefunctionextraction.cpp \
    FunctionExtraction/NdatFileFunctionExtraction/ndatfilefunctionextraction.cpp \
    FunctionExtraction/OldFileFunctionExtraction/oldfilefunctionextraction.cpp \
    FunctionExtraction/OldFileFunctionExtraction/oldfilefunctionextractiondialog.cpp \
    FunctionExtraction/abstractfunctionextraction.cpp \
    FunctionExtraction/filefunctionextractionmanager.cpp \
    experimentanalysisdialog.cpp \
    function.cpp \
    functionapproximation.cpp \
    functionapproximationtabwidget.cpp \
    functionelement.cpp \
    interactableqcustomplot.cpp \
        main.cpp \
        mainwindow.cpp \
    qcustomplot.cpp \
    rangeselectableqcustomplot.cpp

HEADERS += \
    ExperimentAnalyzer/experimentanalysis.h \
    ExperimentAnalyzer/experimentanalyzer.h \
    FunctionApproximator/NetFunctionApproximator/netfunctionapproximator.h \
    FunctionApproximator/abstractfunctionapproximator.h \
    FunctionExtraction/DatFileFunctionExtraction/datfilefunctionextraction.h \
    FunctionExtraction/NdatFileFunctionExtraction/ndatfilefunctionextraction.h \
    FunctionExtraction/OldFileFunctionExtraction/oldfilefunctionextraction.h \
    FunctionExtraction/OldFileFunctionExtraction/oldfilefunctionextractiondialog.h \
    FunctionExtraction/abstractfunctionextraction.h \
    FunctionExtraction/filefunctionextractionmanager.h \
    experimentanalysisdialog.h \
    function.h \
    functionapproximation.h \
    functionapproximationtabwidget.h \
    functionelement.h \
    interactableqcustomplot.h \
        mainwindow.h \
    qcustomplot.h \
    rangeselectableqcustomplot.h

FORMS += \
        FunctionExtraction/OldFileFunctionExtraction/oldfilefunctionextractiondialog.ui \
        experimentanalysisdialog.ui \
        functionapproximationtabwidget.ui \
        interactableqcustomplot.ui \
        mainwindow.ui

win32 {
SOURCES += \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdeviceinterface.cpp \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextraction.cpp \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizard.cpp \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionresources.cpp \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage1choosedevice.cpp \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage2setparameters.cpp \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage3getdata.cpp \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage4writetofile.cpp \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage5finish.cpp

HEADERS += \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdeviceinterface.h \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextraction.h \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizard.h \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionresources.h \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage1choosedevice.h \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage2setparameters.h \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage3getdata.h \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage4writetofile.h \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage5finish.h \
    FunctionExtraction/DAQDeviceFunctionExtraction/oldsp32.h \
    FunctionExtraction/DAQDeviceFunctionExtraction/olerrors.h \
    FunctionExtraction/DAQDeviceFunctionExtraction/olmem.h \
    FunctionExtraction/DAQDeviceFunctionExtraction/oltypes.h \
    FunctionExtraction/DAQDeviceFunctionExtraction/oldaapi.h \
    FunctionExtraction/DAQDeviceFunctionExtraction/oldadefs.h \
    FunctionExtraction/DAQDeviceFunctionExtraction/tedsapi.h

FORMS += \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizard.ui \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage1choosedevice.ui \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage2setparameters.ui \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage3getdata.ui \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage4writetofile.ui \
    FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizardpage5finish.ui

    contains(CONFIG, "qml_debug") {
        GUITEST = yes
    }

    defined(GUITEST, var) {
        SOURCES += FunctionExtraction/DAQDeviceFunctionExtraction/tests/Windows/GUI/testwindowsgui.cpp
        HEADERS += FunctionExtraction/DAQDeviceFunctionExtraction/tests/Windows/GUI/testwindowsgui.h
    } else {
        LIBS += -L$$PWD/FunctionExtraction/DAQDeviceFunctionExtraction/libs/
        contains(QT_ARCH, i386) {
            LIBS += -loldaapi32 -loldsp32 -lolmem32 -lolmemsup
        } else {
            LIBS += -loldaapi64 -lolmem64
        }
    }
}
