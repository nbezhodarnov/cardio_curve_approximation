#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H

#include <cmath>

#include "function.h"

#define PRECISION 0.001

// Вычисление результата скользящего среднего
Function getMA(const Function &function, const int &size);

double findKeyByValue(const std::function<double (const double&)> &function, const double &value, const double &precision, const double &start);

double findMaximum(const std::function<double (const double&)> &function, const double &precision, const double &start);

double findIntegral(const std::function<double (const double&)> &function, const double &start, const double &end, const double &precision);

#endif // MATHFUNCTIONS_H
