#include <cstring>
#include <windows.h>
#include <QMessageBox>
#include <QStringList>

#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionresources.h"

#define STR_LENGTH 80

int DisplayErrorMessage(const char* str) {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Ошибка!");
    msgBox.setText(str);
    return msgBox.exec();
}

BOOL CALLBACK AddBoardList(PTSTR lpszName, PTSTR lpszEntry, LPARAM lParam) {
    wchar_t str[STR_LENGTH];
    wsprintf(str, L"%s (%s)", lpszName, lpszEntry);
    ((QStringList*)lParam)->append(QString::fromWCharArray(str));
    return true;
}

BOOL CALLBACK AddRangeList(UINT cap, DBL max, DBL min, LPARAM lParam) {
    Q_UNUSED(cap);
    char str[STR_LENGTH];
    //sprintf(str, RANGE_FORMAT, min, max);
    sprintf(str, RANGE2_FORMAT, min, max);
    ((QStringList*)lParam)->append(str);
    return true;
}

BOOL CALLBACK AddGainList(UINT cap, DBL gain, DBL unused, LPARAM lParam) {
    Q_UNUSED(cap);
    Q_UNUSED(unused);
    char str[STR_LENGTH];
    sprintf(str, GAIN_FORMAT, gain);
    ((QStringList*)lParam)->append(str);
    return true;
}

BOOL CALLBACK AddFilterList(UINT cap, DBL filter, DBL unused, LPARAM lParam) {
    Q_UNUSED(cap);
    Q_UNUSED(unused);
    char str[STR_LENGTH];
    sprintf(str, FILTER_FORMAT, filter);
    ((QStringList*)lParam)->append(str);
    return true;
}
