#include <QMessageBox>

#include "oldfilefunctionextractiondialog.h"
#include "ui_oldfilefunctionextractiondialog.h"

OldFileFunctionExtractionDialog::OldFileFunctionExtractionDialog(QFile *ptr, QWidget *parent) : QDialog(parent), ui(new Ui::OldFileFunctionExtractionDialog), file(ptr) {
    ui->setupUi(this);
    if (!file->isOpen()) {
        file->open(QIODevice::ReadOnly);
    }
    ChangeChannelsCount(1);
}

OldFileFunctionExtractionDialog::~OldFileFunctionExtractionDialog() {
    delete ui;
}

qint64 OldFileFunctionExtractionDialog::GetExperimentNumber() {
    return ui->experiment_choose->value();
}

qint64 OldFileFunctionExtractionDialog::GetChannelNumber() {
    return ui->channel_choose->value();
}

void OldFileFunctionExtractionDialog::ChangeExperimentsCount(qint64 experiments_count) {
    ui->experiment_choose->setRange(1, experiments_count);
}

void OldFileFunctionExtractionDialog::on_make_func_clicked() {
    done(QMessageBox::Ok);
}

void OldFileFunctionExtractionDialog::ChangeChannelsCount(qint64 experiment) {
    if (!file->isOpen()) {
        return;
    }
    qint64 file_size = file->size() - 1;
    qint8 input_number = 0, f[256];

    file->seek(257 + (experiment - 1) * 768);
    for (quint8 i = 0; i < 20; ++i) {
        file->read((char *) &f[0], 1);
        if ((f[0] != 63) && (f[0] != 0)) {
            break;
        }
    }
    if ((f[0] == 63) || (f[0] == 0) || ((file->atEnd()) && (file_size % 768 > 0))) {
        input_number = 1;
    } else {
        file->seek(513 + (experiment - 1) * 768);
        for (quint8 i = 0; i < 20; ++i) {
            file->read((char *) &f[0], 1);
            if ((f[0] != 63) && (f[0] != 0)) {
                break;
            }
        }
        if ((f[0] == 63) || (f[0] == 0)  || ((file->atEnd()) && (file_size % 768 > 0))) {
            input_number = 2;
        } else {
            input_number = 3;
        }
    }

    ui->channel_choose->setRange(1, input_number);
}

void OldFileFunctionExtractionDialog::on_experiment_choose_valueChanged(int arg1) {
    ChangeChannelsCount(arg1);
}
