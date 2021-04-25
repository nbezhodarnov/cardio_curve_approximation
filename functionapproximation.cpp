#include <math.h>

#include "functionapproximation.h"

FunctionApproximation::FunctionApproximation(const double &a1, const double &b1, const double &c1, const double &a2, const double &b2, const double &c2, const double &constant) {
    coefficients.resize(7);
    set(a1, b1, c1, a2, b2, c2, constant);
}

FunctionApproximation::FunctionApproximation(const QVector<double> &coefficients_input) {
    set(coefficients_input);
}

void FunctionApproximation::set(const double &a1, const double &b1, const double &c1, const double &a2, const double &b2, const double &c2, const double &constant) {
    coefficients[0] = a1;
    coefficients[1] = b1;
    coefficients[2] = c1;
    coefficients[3] = a2;
    coefficients[4] = b2;
    coefficients[5] = c2;
    coefficients[6] = constant;
}

void FunctionApproximation::set(const QVector<double> &coefficients_input) {
    coefficients = coefficients_input;
    coefficients.resize(7);
}

QVector<double> FunctionApproximation::getCoefficients() const {
    return coefficients;
}

double FunctionApproximation::getValue(const double &x) const {
    return coefficients[0] * exp(-coefficients[1] * pow(x - coefficients[2], 2)) + coefficients[3] * exp(-coefficients[4] * pow(x - coefficients[5], 2)) + coefficients[6];
}

double FunctionApproximation::getFirstComponentValue(const double &x) const {
    return coefficients[0] * exp(-coefficients[1] * pow(x - coefficients[2], 2));
}

double FunctionApproximation::getSecondComponentValue(const double &x) const {
    return coefficients[3] * exp(-coefficients[4] * pow(x - coefficients[5], 2));
}

double FunctionApproximation::getFirstDerivativeValue(const double &x) const {
    return (-2 * coefficients[0] * coefficients[1]) * (x - coefficients[2]) * exp(-coefficients[1] * pow(x - coefficients[2], 2)) + (-2 * coefficients[3] * coefficients[4]) * (x - coefficients[5]) * exp(-coefficients[4] * pow(x - coefficients[5], 2));
}

double FunctionApproximation::getSecondDerivativeValue(const double &x) const {
    return (-2 * coefficients[0] * coefficients[1]) * exp(-coefficients[1] * pow(x - coefficients[2], 2)) + (4 * coefficients[0] * pow(coefficients[1], 2)) * pow(x - coefficients[2], 2) * exp(-coefficients[1] * pow(x - coefficients[2], 2)) + (-2 * coefficients[3] * coefficients[4]) * exp(-coefficients[4] * pow(x - coefficients[5], 2)) + (4 * coefficients[3] * pow(coefficients[4], 2)) * pow(x - coefficients[5], 2) * exp(-coefficients[4] + pow(x - coefficients[5], 2));
}

double FunctionApproximation::getConstant() const {
    return coefficients[6];
}
