#include <QMessageBox>

#include "daqdevicefunctionextraction.h"
#include "FunctionExtraction/DAQDeviceFunctionExtraction/daqdevicefunctionextractionwizard.h"

DAQDeviceFunctionExtraction::DAQDeviceFunctionExtraction(QWidget *ptr): AbstractFunctionExtraction(ptr) {
}

Function DAQDeviceFunctionExtraction::extract() {
    DAQDeviceFunctionExtractionWizard wizard(parent);
    wizard.SetDAQDevicePtr(&device_interface);
    wizard.SetFunctionPtr(&function);
    if ((wizard.exec() != QDialog::Accepted) || (function.empty())) {
        return Function();
    }
    Function result;
    for (QList<FunctionElement>::iterator iterator = function.begin(); iterator != function.end(); iterator = function.erase(iterator)) {
        result.add(*iterator);
    }
    return result;
}
