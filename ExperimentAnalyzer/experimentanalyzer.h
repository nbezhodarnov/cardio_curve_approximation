#ifndef EXPERIMENTANALYZER_H
#define EXPERIMENTANALYZER_H

#include <memory>

#include <QVector>

#include "experimentanalysis.h"
#include "FunctionApproximator/abstractfunctionapproximator.h"

class ExperimentAnalyzer
{
public:
    ExperimentAnalyzer(std::unique_ptr<AbstractFunctionApproximator> functionApproximator);
    ExperimentAnalysis analyze(const Function &experiment) const;

private:
    Function getMA(const Function &functon, const int &size) const;
    Function removeNoise(const Function &experiment) const;
    QVector<Function> extractOscillations(const Function &experiment) const;
    QVector<FunctionApproximation> calculateApproximations(const QVector<Function> &oscillations) const;
    ExperimentAnalysis formAnalysis(const QVector<FunctionApproximation> &oscillations) const;

    std::unique_ptr<AbstractFunctionApproximator> approximator;
};

#endif // EXPERIMENTANALYZER_H
