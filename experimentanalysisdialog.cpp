#include "experimentanalysisdialog.h"
#include "ui_experimentanalysisdialog.h"
#include "functionapproximationtabwidget.h"

ExperimentAnalysisDialog::ExperimentAnalysisDialog(ExperimentAnalysis newAnalysis, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExperimentAnalysisDialog),
    analysis(newAnalysis)
{
    ui->setupUi(this);
    ui->dynamicsPlot->plotFunction(analysis.getDymanics());
}

ExperimentAnalysisDialog::~ExperimentAnalysisDialog()
{
    delete ui;
}

#define PRECISION 0.001

void ExperimentAnalysisDialog::on_exportPushButton_clicked()
{
    QString file_name = QFileDialog::getSaveFileName(this, tr("Сохранить результаты"), "", tr("Результаты (*.nar);;Все файлы (*)"));
    if (file_name.isEmpty()) {
        //std::cerr << "No file name has been provided!\n";
        return;
    }
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(QString::fromUtf8("Ошибка"));
        msgBox.setText(QString::fromUtf8("Произошла ошибка при записи результатов в файл!"));
        msgBox.exec();
        return;
    }
    for (const FunctionApproximation &approximation : analysis.getOscillations()) {
        QVector<double> coefficients = approximation.getCoefficients();
        bool are_two_peaks = true;
        if (coefficients[3] == 0) {
            are_two_peaks = false;
        }
        double constant = approximation.getConstant();
        double start = findKeyByValue([&approximation](const double &key) {return approximation.getValue(key);}, constant, PRECISION, coefficients[2]);
        double end = findKeyByValue([&approximation](const double &key) {return approximation.getValue(key);}, constant, PRECISION, coefficients[5]);
        if (!are_two_peaks) {
            end = findKeyByValue([&approximation](const double &key) {return approximation.getValue(key);}, constant, PRECISION, coefficients[2] + 0.5);
        }

        QString result =
                QString::fromUtf8("Результаты вычислений:\n") +
                QString::fromUtf8("Коэффициенты при экспонентах:\na1 = ") + QString::number(coefficients[0]) +
                QString::fromUtf8(", b1 = ") + QString::number(coefficients[1]) +
                QString::fromUtf8(", c1 = ") + QString::number(coefficients[2]) +
                QString::fromUtf8("\na2 = ") + QString::number(coefficients[3]) +
                QString::fromUtf8(", b2 = ") + QString::number(coefficients[4]) +
                QString::fromUtf8(", c2 = ") + QString::number(coefficients[5]) +
                QString::fromUtf8("\nconst = ") + QString::number(coefficients[6]) +
                QString::fromUtf8("\nПервая производная:\n") +
                QString::number(-2 * coefficients[0] * coefficients[1]) +
                QString::fromUtf8("(x-") +
                QString::number(coefficients[2]) +
                QString::fromUtf8(")exp{") +
                QString::number(-coefficients[1]) +
                QString::fromUtf8("(x-") +
                QString::number(coefficients[2]) +
                QString::fromUtf8(")^2}") +
                QString::number(-2 * coefficients[3] * coefficients[4]) +
                QString::fromUtf8("(x-") + QString::number(coefficients[5]) +
                QString::fromUtf8(")exp{") +
                QString::number(-coefficients[4]) +
                QString::fromUtf8("(x-") +
                QString::number(coefficients[5]) +
                QString::fromUtf8(")^2}") +
                QString::fromUtf8("\nВторая производная:\n") +
                QString::fromUtf8("2(2*") +
                QString::number(coefficients[0]) +
                QString::fromUtf8("*") +
                QString::number(coefficients[1]) +
                QString::fromUtf8("^(2)*(x-") +
                QString::number(coefficients[2]) +
                QString::fromUtf8(")^(2)*") +
                QString::fromUtf8("exp{") +
                QString::number(-coefficients[1]) +
                QString::fromUtf8("(x-") +
                QString::number(coefficients[2]) +
                QString::fromUtf8(")^2}") +
                QString::fromUtf8("-") +
                QString::number(coefficients[0]) +
                QString::fromUtf8("*") +
                QString::number(coefficients[1]) +
                QString::fromUtf8("*exp{") +
                QString::number(-coefficients[1]) +
                QString::fromUtf8("(x-") +
                QString::number(coefficients[2]) +
                QString::fromUtf8(")^2}") +
                QString::fromUtf8("+2*") +
                QString::number(coefficients[3]) +
                QString::fromUtf8("*") +
                QString::number(coefficients[4]) +
                QString::fromUtf8("^(2)*(x-") +
                QString::number(coefficients[5]) +
                QString::fromUtf8(")^(2)*") +
                QString::fromUtf8("exp{") +
                QString::number(-coefficients[4]) +
                QString::fromUtf8("(x-") +
                QString::number(coefficients[5]) +
                QString::fromUtf8(")^2}") +
                QString::fromUtf8("-") +
                QString::number(coefficients[3]) +
                QString::fromUtf8("*") +
                QString::number(coefficients[4]) +
                QString::fromUtf8("*exp{") +
                QString::number(-coefficients[4]) +
                QString::fromUtf8("(x-") +
                QString::number(coefficients[5]) +
                QString::fromUtf8(")^2})");

        double peak_time_1 = 0, peak_time_2 = 0;
        peak_time_1 = findMaximum([&approximation](const double &key){return approximation.getValue(key);}, PRECISION, coefficients[2]);
        peak_time_2 = findMaximum([&approximation](const double &key){return approximation.getValue(key);}, PRECISION, coefficients[5]);
        if (are_two_peaks && std::abs(peak_time_2 - peak_time_1) < PRECISION * 50) {
            are_two_peaks = false;
            peak_time_2 = peak_time_1;
        } else if (!are_two_peaks) {
            peak_time_2 = peak_time_1;
        }
        double first_peak_maximum = approximation.getValue(peak_time_1), second_peak_maximim = -INFINITY;

        double amplitude = first_peak_maximum - constant;
        double amplitude_2 = first_peak_maximum - constant;

        double time_up_50_percent = -INFINITY;
        double time_down_50_percent = -INFINITY;

        double time_up_90_percent = -INFINITY;
        double time_down_90_percent = -INFINITY;

        double time_up_10_percent = -INFINITY;
        double time_down_10_percent = -INFINITY;

        QString additional_info_text = "";
        additional_info_text +=
                QString::fromUtf8("Момент первого пика: ") +
                QString::number(peak_time_1) +
                QString::fromUtf8("\nЗначение в первом пике: ") +
                QString::number(first_peak_maximum) + '\n';

        if (are_two_peaks) {
            second_peak_maximim = approximation.getValue(peak_time_2);
            additional_info_text +=
                    QString::fromUtf8("Момент второго пика: ") +
                    QString::number(peak_time_2) +
                    QString::fromUtf8("\nЗначение во втором пике: ") +
                    QString::number(second_peak_maximim) + '\n';
            amplitude_2 = second_peak_maximim - constant;
            time_down_50_percent = findKeyByValue([&approximation](const double &key) {return approximation.getValue(key);}, second_peak_maximim - amplitude_2 / 2.0, PRECISION, end);
            time_down_90_percent = findKeyByValue([&approximation](const double &key) {return approximation.getValue(key);}, second_peak_maximim - amplitude_2 / 10.0, PRECISION, end);
            time_down_10_percent = findKeyByValue([&approximation](const double &key) {return approximation.getValue(key);}, second_peak_maximim - 9.0 * amplitude_2 / 10.0, PRECISION, end);
        } else {
            if (first_peak_maximum < second_peak_maximim) {
                peak_time_1 = peak_time_2;
                first_peak_maximum = second_peak_maximim;
                amplitude = first_peak_maximum - constant;
                amplitude_2 = first_peak_maximum - constant;
            }
            time_down_50_percent = findKeyByValue([&approximation](const double &key) {return approximation.getValue(key);}, first_peak_maximum - amplitude / 2.0, PRECISION, end);
            time_down_90_percent = findKeyByValue([&approximation](const double &key) {return approximation.getValue(key);}, first_peak_maximum - amplitude / 10.0, PRECISION, end);
            time_down_10_percent = findKeyByValue([&approximation](const double &key) {return approximation.getValue(key);}, first_peak_maximum - 9.0 * amplitude / 10.0, PRECISION, end);
        }

        time_up_50_percent = findKeyByValue([&approximation](const double &key) {return approximation.getValue(key);}, first_peak_maximum - amplitude / 2.0, PRECISION, start);

        time_up_90_percent = findKeyByValue([&approximation](const double &key) {return approximation.getValue(key);}, first_peak_maximum - amplitude / 10.0, PRECISION, start);

        time_up_10_percent = findKeyByValue([&approximation](const double &key) {return approximation.getValue(key);}, first_peak_maximum - 9.0 * amplitude / 10.0, PRECISION, start);

        additional_info_text +=
                QString::fromUtf8("Значение во время простоя: ") +
                QString::number(constant) + '\n';

        additional_info_text +=
                QString::fromUtf8("Амплитуда: ") +
                QString::number(amplitude) + '\n';

        additional_info_text +=
                QString::fromUtf8("Момент 50%-ной амплитуды на подъёме: ") +
                QString::number(time_up_50_percent) + '\n';
        additional_info_text +=
                QString::fromUtf8("Значение 50%-ной амплитуды на подъёме: ") +
                QString::number(approximation.getValue(time_up_50_percent)) + '\n';

        additional_info_text +=
                QString::fromUtf8("Момент 50%-ной амплитуды на спуске: ") +
                QString::number(time_down_50_percent) + '\n';
        additional_info_text +=
                QString::fromUtf8("Значение 50%-ной амплитуды на спуске: ") +
                QString::number(approximation.getValue(time_down_50_percent)) + '\n';

        double velocity_max_up_time = findMaximum([&approximation](const double &key){return approximation.getFirstDerivativeValue(key);}, PRECISION, peak_time_1 - (peak_time_1 - start) / 2.0);

        double velocity_max_down_time = findMaximum([&approximation](const double &key){return -approximation.getFirstDerivativeValue(key);}, PRECISION, peak_time_2 + (end - peak_time_2) / 2.0);

        additional_info_text +=
                QString::fromUtf8("Максимальная скорость на подъёме: ") +
                QString::number(approximation.getFirstDerivativeValue(velocity_max_up_time)) + '\n';
        additional_info_text +=
                QString::fromUtf8("Максимальная скорость на спуске: ") +
                QString::number(-approximation.getFirstDerivativeValue(velocity_max_down_time)) + '\n';

        additional_info_text +=
                QString::fromUtf8("Скорость на подъёме (тип 1): ") +
                QString::number(amplitude / (peak_time_1 - start)) + '\n';

        additional_info_text +=
                QString::fromUtf8("Скорость на спуске (тип 1): ") +
                QString::number(amplitude_2 / (end - peak_time_2)) + '\n';

        double time = end - start;

        additional_info_text +=
                QString::fromUtf8("Полное время: ") +
                QString::number(time) + '\n';

        double integral = findIntegral([&approximation](const double &key) {return approximation.getValue(key);}, start, end, PRECISION);

        additional_info_text +=
                QString::fromUtf8("Интеграл: ") +
                QString::number(integral) + '\n';

        additional_info_text +=
                QString::fromUtf8("Скорость на подъёме (тип 2): ") +
                QString::number((approximation.getValue(time_up_90_percent) - approximation.getValue(time_up_10_percent)) / (time_up_90_percent - time_up_10_percent)) + '\n';

        additional_info_text +=
                QString::fromUtf8("Скорость на спуске (тип 2): ") +
                QString::number((approximation.getValue(time_down_90_percent) - approximation.getValue(time_down_10_percent)) / (time_down_10_percent - time_down_90_percent)) + '\n';


        result += "\n\n" + additional_info_text + "\n\n";
        file.write(result.toUtf8());
    }
    file.close();
    QMessageBox msgBox;
    msgBox.setWindowTitle(QString::fromUtf8("Успех"));
    msgBox.setText(QString::fromUtf8("Результаты успешно сохранены!"));
    msgBox.exec();
}

