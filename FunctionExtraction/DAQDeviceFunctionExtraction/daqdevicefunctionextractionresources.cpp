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
    char str[STR_LENGTH];
    char *name = (char*)lpszName, *entry = (char*)lpszEntry;
    unsigned int name_index = 0, str_index = 0;
    while (name[name_index] != '\0') {
        str[str_index] = name[name_index];
        str_index++;
        name_index++;
    }
    str[str_index] = ' ';
    str_index++;
    str[str_index] = '(';
    str_index++;
    unsigned int entry_index = 0;
    while (entry[entry_index] != '\0') {
        str[str_index] = entry[entry_index];
        str_index++;
        entry_index++;
    }
    str[str_index] = ')';
    str_index++;
    str[str_index] = '\0';
    str_index++;
    ((QStringList*)lParam)->append(QString::fromUtf8(str));
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
