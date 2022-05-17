#ifndef EXPERIMENTANALYSIS_H
#define EXPERIMENTANALYSIS_H

#include <QVector>

#include "function.h"
#include "functionapproximation.h"

struct OscillationAnalysis {
    double secondComponentMaxValue;
    FunctionApproximation oscillation;
};

class ExperimentAnalysis
{
public:
    ExperimentAnalysis(const QVector<FunctionApproximation> &oscillations = {});
    void append(const FunctionApproximation &oscillation);
    const FunctionApproximation& getOscillation(unsigned int index) const;
    Function getDymanics() const;

private:
    double findFunctionMaxValue(const std::function<double (const double&)> &function, const double &start, const double &precision = 0.001) const;
    double findSecondComponentMaxValue(const FunctionApproximation &oscillation) const;

    QVector<OscillationAnalysis> analyzes;
};

#endif // EXPERIMENTANALYSIS_H
