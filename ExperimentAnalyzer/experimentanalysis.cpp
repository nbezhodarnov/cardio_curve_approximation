#include "experimentanalysis.h"

#include <cmath>

ExperimentAnalysis::ExperimentAnalysis(const QVector<FunctionApproximation> &oscillations)
{
    for (const FunctionApproximation &oscillation : oscillations) {
        append(oscillation);
    }
}

void ExperimentAnalysis::append(const FunctionApproximation &oscillation)
{
    double secondComponentMaxValue = findSecondComponentMaxValue(oscillation);
    analyzes.append({secondComponentMaxValue, oscillation});
}

const FunctionApproximation &ExperimentAnalysis::getOscillation(unsigned int index) const
{
    if (index >= (unsigned int)analyzes.size()) {
        throw std::runtime_error("Trying to access unexisted oscillation!");
    }
    return analyzes[index].oscillation;
}

Function ExperimentAnalysis::getDymanics() const
{
    Function dynamics;
    for (int i = 0; i < analyzes.size(); i++) {
        dynamics.add({static_cast<double>(i), analyzes[i].secondComponentMaxValue});
    }
    return dynamics;
}

double ExperimentAnalysis::findFunctionMaxValue(const std::function<double (const double&)> &function, const double &start, const double &precision) const
{
    double key = start;
    double step = 0.1;
    double step_left_value = -INFINITY, in_key_value = -INFINITY, step_right_value = -INFINITY;
    double max_difference = INFINITY;
    double max = -INFINITY;
    while (max_difference > precision) {
        step_left_value = function(key - step);
        in_key_value = function(key);
        step_right_value = function(key + step);
        max = std::max({step_left_value, in_key_value, step_right_value});
        if (max == step_left_value) {
            key -= step;
        } else if (max == in_key_value) {
            step /= 10.0;
        } else {
            key += step;
        }
        max_difference = std::max({max - step_left_value, max - in_key_value, max - step_right_value});
    }
    return key;
}

double ExperimentAnalysis::findSecondComponentMaxValue(const FunctionApproximation &oscillation) const
{
    std::function<double (const double&)> function = [&oscillation](const double &key){return oscillation.getSecondComponentValue(key);};
    QVector<double> coefficients = oscillation.getCoefficients();
    double startValue = coefficients[5];
    return findFunctionMaxValue(function, startValue);
}
