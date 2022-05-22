#include <math.h>
#include <QMessageBox>
#include <QTextStream>
#include <QApplication>

#include "netfunctionapproximator.h"

inline LineIndexes::LineIndexes(unsigned int begin_input, unsigned int end_input) {
    begin = begin_input;
    end = end_input;
}

inline bool LineIndexes::isIndexInLine(unsigned int index) {
    return ((index >= begin) && (index <= end));
}

inline bool LineIndexes::isIndexOutOfLine(unsigned int index) {
    return ((index < begin) || (index > end));
}

NetFunctionApproximator::NetFunctionApproximator(QWidget *ptr): AbstractFunctionApproximator(ptr) {
}

FunctionApproximation NetFunctionApproximator::approximate(const Function &function, const std::array<double, 3> &firstComponent) {
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(QString::fromUtf8("Пожалуйста, подождите"));
    msgBox.setText(QString::fromUtf8("Пожалуйста, подождите. Идут вычисления."));
    msgBox.setStandardButtons(QMessageBox::NoButton);
    msgBox.setModal(true);
    msgBox.show();
    QApplication::processEvents();

    QTextStream out(stdout);

    double constant = findConstant(function);
    Function normalized_function = normalizeFunction(function, constant);
    FunctionApproximation approximation = calculateApproximation(normalized_function, firstComponent);

    approximation.setConstant(constant);

    return approximation;
}

double NetFunctionApproximator::findConstant(const Function &function) const
{
    unsigned int min_local = 0, points_count = function.size();
    for (unsigned int i = 1; i < points_count; ++i) {
        if (function.getValue(i) < function.getValue(min_local)) {
            min_local = i;
        }
    }
    double coefficient = function.getValue(min_local);
    return coefficient;
}

Function NetFunctionApproximator::normalizeFunction(const Function &function, const double &coefficient) const
{
    unsigned int points_count = function.size();
    Function normalized_function;
    for (unsigned int i = 0; i < points_count; ++i) {
        normalized_function.add({function.getKey(i), function.getValue(i) - coefficient});
    }
    return normalized_function;
}

FunctionApproximation NetFunctionApproximator::calculateStartPoint(const Function &function, const std::array<double, 3> &firstComponent) const
{
    unsigned int points_count = function.size();
    unsigned int index = points_count * 0.2;
    double a1, a2, b1, b2, c1, c2, step = function.getStep();
    while (function.getValue(index) <= function.getValue(index + 1)) {
        index++;
    }
    c1 = function.getKey(index);
    a1 = function.getValue(index);
    b1 = std::abs(-log(function.getValue(index / 2) / a1) / (pow(function.getKey(index / 2) - c1, 2)));

    index = int(points_count * 0.8);
    while (function.getValue(index) <= function.getValue(index - 1)) {
        index--;
    }
    c2 = function.getKey(index);
    a2 = function.getValue(index);
    b2 = std::abs(-log(function.getValue(index + (points_count - index) / 2) / a2) / (pow(function.getKey(index + (points_count - index) / 2) - c2, 2)));

    bool are_two_extremums = true;
    if (std::abs(c1 - c2) <= 25 * step) {
        are_two_extremums = false;
    }

    bool is_first_component_set = false;
    if (firstComponent[0] != 0) {
        is_first_component_set = true;
    }

    if (!are_two_extremums) {
        a1 /= 2;
        a2 = a1;
        b2 = b1;
        c1 -= step * 25;
        c2 += step * 25;
    }

    if (is_first_component_set) {
        a1 = std::abs(firstComponent[0]);
        b1 = std::abs(firstComponent[1]);
        c1 = firstComponent[2];
        if (std::abs(c1 - c2) < 50 * step) {
            c2 = c1 + 50 * step;
        }
    }

    return FunctionApproximation(a1, b1, c1, a2, b2, c2);
}

bool NetFunctionApproximator::isValidApproximation(const Function &function, const FunctionApproximation &approximation) const
{
    unsigned int points_count = function.size();
    double step = function.getStep();
    double first_component_value_in_center = approximation.getFirstComponentValue(function.getKey(points_count / 2));
    double second_component_value_in_center = approximation.getSecondComponentValue(function.getKey(points_count / 2));
    if (first_component_value_in_center < function.getValue(points_count / 2) * 0.1) {
        return false;
    }
    if (second_component_value_in_center < function.getValue(points_count / 2) * 0.1) {
        return false;
    }

    QVector<double> approximationCoefficients = approximation.getCoefficients();
    if (std::abs(approximationCoefficients[2] - approximationCoefficients[5]) < step * 30) {
        return false;
    }
    if (function.getKey(0) > approximationCoefficients[2] || approximationCoefficients[2] > function.getKey(points_count - 1)) {
        return false;
    }
    if (function.getKey(0) > approximationCoefficients[5] || approximationCoefficients[5] > function.getKey(points_count - 1)) {
        return false;
    }
    return true;
}

double NetFunctionApproximator::calculateDifference(const Function &function, const FunctionApproximation &approximation) const
{
    double difference = 0;
    unsigned int points_count = function.size();
    for (unsigned int i = 0; i < points_count; ++i) {
        double first_component_value = approximation.getFirstComponentValue(function.getKey(i));
        double second_component_value = approximation.getSecondComponentValue(function.getKey(i));
        double approximating_function = first_component_value + second_component_value;
        difference += std::abs(function.getValue(i) - approximating_function);
    }
    return difference;
}

FunctionApproximation NetFunctionApproximator::calculateApproximation(const Function &function, const std::array<double, 3> &firstComponent) const
{
    QTextStream out(stdout);

    FunctionApproximation startPoint = calculateStartPoint(function, firstComponent);

    QVector<double> steps = {10, 50, 10};
    double &step1 = steps[0];
    double &step2 = steps[1];
    double &step3 = steps[2];

    unsigned int points_count = function.size();
    double step = function.getStep();

    QVector<double> coefficients = startPoint.getCoefficients();

    out << "a1 = " << coefficients[0] << ", b1 = " << coefficients[1] << ", c1 = " << coefficients[2] << '\n' << flush;
    out << "a2 = " << coefficients[3] << ", b2 = " << coefficients[4] << ", c2 = " << coefficients[5] << "\n\n" << flush;

    bool is_first_component_set = false;
    if (firstComponent[0] != 0) {
        is_first_component_set = true;
    }

    unsigned int firstEditableComponentIndex = 0;
    if (is_first_component_set) {
        coefficients[0] = std::abs(firstComponent[0]);
        coefficients[1] = std::abs(firstComponent[1]);
        coefficients[2] = firstComponent[2];
        firstEditableComponentIndex = 1;
    }

    FunctionApproximation previousBestApproximation(coefficients);
    FunctionApproximation bestApproximation(coefficients);
    uint8_t little_changes_count = 0, big_changes_count = 0, max_little_changes = 10, big_changes_trigger = 10;
    double min_difference = INFINITY, previous_difference = 0, difference = 0;
    while (min_difference > step) {
        if (!is_first_component_set && (coefficients[2] < function.getKey(0) - 1000 * step || coefficients[2] > function.getKey(points_count - 1) + 1000 * step)) {
            coefficients[0] = coefficients[1] = 0;
            coefficients[2] = function.getKey(0);
        }
        if (coefficients[5] < function.getKey(0) - 1000 * step || coefficients[5] > function.getKey(points_count - 1) + 1000 * step) {
            coefficients[3] = coefficients[4] = 0;
            coefficients[5] = function.getKey(points_count - 1);
        }
        if (std::abs(previous_difference - min_difference) < step) {
            little_changes_count++;
            if (little_changes_count > max_little_changes) {
                break;
            }
        } else {
            little_changes_count = 0;
        }
        previous_difference = min_difference;
        previousBestApproximation = bestApproximation;
        uint8_t net_step = 3, net_step_center = net_step / 2;

        //QList<FunctionApproximation> approximations = {};
        QVector<QList<std::array<double, 3>>> componentsVariants(coefficients.size() / 3);
        if (is_first_component_set) {
            componentsVariants[0].append({coefficients[0], coefficients[1], coefficients[2]});
        }
        for (uint8_t i = firstEditableComponentIndex; i < componentsVariants.size(); i++) {
            QVector<double> coefficientsVariants[3];
            for (uint8_t j = 0; j < 3; j++) {
                for (uint8_t intend = 0; intend < net_step; intend++) {
                    coefficientsVariants[j].push_back(coefficients[i * 3 + j] - (net_step_center - intend) * steps[j]);
                }
            }
            for (uint8_t j = 0; j < coefficientsVariants[0].size(); j++) {
                for (uint8_t k = 0; k < coefficientsVariants[1].size(); k++) {
                    for (uint8_t l = 0; l < coefficientsVariants[2].size(); l++) {
                        componentsVariants[i].push_back({coefficientsVariants[0][j], coefficientsVariants[1][k], coefficientsVariants[2][l]});
                    }
                }
            }
        }
        int variantsCount = 1;
        for (const auto &componentVariants : componentsVariants) {
            variantsCount *= componentVariants.size();
        }
        QVector<int> componentVariantIndexes(componentsVariants.size());
        for (int i = 0; i < variantsCount; i++) {
            int variantIndex = i;
            for (int j = componentsVariants.size() - 1; j >= 0; j--) {
                componentVariantIndexes[j] = variantIndex % componentsVariants[j].size();
                variantIndex /= componentsVariants[j].size();
            }
            QVector<double> approximationVariantCoefficients(componentsVariants.size() * 3);
            for (int k = 0; k < componentVariantIndexes.size(); k++) {
                for (unsigned int l = 0; l < componentsVariants[k][componentVariantIndexes[k]].size(); l++) {
                    approximationVariantCoefficients[k * 3 + l] = componentsVariants[k][componentVariantIndexes[k]][l];
                }
            }
            //approximations.append(FunctionApproximation(approximationVariantCoefficients));
            FunctionApproximation approximation(approximationVariantCoefficients);
            if (!isValidApproximation(function, approximation)) {
                continue;
            }
            difference = calculateDifference(function, approximation);
            if (difference < min_difference) {
                min_difference = difference;
                bestApproximation = approximation;
            }
        }

        coefficients = bestApproximation.getCoefficients();

        out << "a1 = " << coefficients[0] << ", b1 = " << coefficients[1] << ", c1 = " << coefficients[2] << '\n' << flush;
        out << "a2 = " << coefficients[3] << ", b2 = " << coefficients[4] << ", c2 = " << coefficients[5] << "\n\n" << flush;
        out << min_difference << ' ' << previous_difference << '\n' << flush;

        out << '\n' << step1 << ' ' << step2 << ' ' << step3 << '\n' << flush;
        out << "\n\n" << flush;

        if (previousBestApproximation == bestApproximation) {
            step1 /= 10;
            step2 /= 10;
            step3 /= 10;
        } else {
            big_changes_count++;
            if (big_changes_count > big_changes_trigger) {
                step1 *= 10;
                step2 *= 10;
                step3 *= 10;
                big_changes_count = 0;
            }
        }
    }

    coefficients[0] = std::abs(coefficients[0]);
    coefficients[3] = std::abs(coefficients[3]);
    coefficients[1] = std::abs(coefficients[1]);
    coefficients[4] = std::abs(coefficients[4]);

    if (coefficients[5] < function.getKey(0) - 100 * step || coefficients[5] > function.getKey(points_count - 1) + 100 * step) {
        coefficients[3] = coefficients[4] = coefficients[5] = 0;
    } else if (coefficients[2] > coefficients[5]) {
        std::swap(coefficients[0], coefficients[3]);
        std::swap(coefficients[1], coefficients[4]);
        std::swap(coefficients[2], coefficients[5]);
    }

    out << "a1 = " << coefficients[0] << ", b1 = " << coefficients[1] << ", c1 = " << coefficients[2] << '\n' << flush;
    out << "a2 = " << coefficients[3] << ", b2 = " << coefficients[4] << ", c2 = " << coefficients[5] << "\n\n" << flush;

    return bestApproximation;
}
