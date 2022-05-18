#include "experimentanalyzer.h"

ExperimentAnalyzer::ExperimentAnalyzer(std::unique_ptr<AbstractFunctionApproximator> &functionApproximator) :
    approximator(std::move(functionApproximator))
{

}

ExperimentAnalysis ExperimentAnalyzer::analyze(const Function &experiment) const
{
    Function experimentWithoutNoise = removeNoise(experiment);
    QVector<Function> oscillations = extractOscillations(experimentWithoutNoise);
    QVector<FunctionApproximation> oscillationsApproximation = calculateApproximations(oscillations);
    ExperimentAnalysis analysis = formAnalysis(oscillationsApproximation);
    return analysis;
}

// Вычисление результата скользящего среднего
Function ExperimentAnalyzer::getMA(const Function &function, const int &size) const {
    Function result = {};
    // size - количество отсчетов интервала усреднения
    double sumx = 0; // сумма отсчетов на интервале
    double *mas = new double[size]; // массив для хранения size отсчетов
    int index = 0; // индекс элемента массива
    for (int i = 0; i < size; i++) mas[i] = 0;
    for (unsigned int i = 0; i < function.size(); i++) {
        sumx -= mas[index];
        mas[index] = function.getValue(i);
        sumx += mas[index];
        index++;
        if (index >= size)
            index = 0; // возврат к началу "окна"
        result.add({function.getKey(i), sumx / size});
    }
    delete []mas;
    return result;
}

Function ExperimentAnalyzer::removeNoise(const Function &experiment) const
{
    unsigned int pointsInterval = 15;
    Function experimentWithoutNoise = getMA(experiment, pointsInterval);
    experimentWithoutNoise.removeElementsFromBeginning(pointsInterval);
    return experimentWithoutNoise;
}

QVector<Function> ExperimentAnalyzer::extractOscillations(const Function &experiment) const
{
    QVector<Function> oscillations = {};
    unsigned int index = 1;
    while (index < experiment.size()) {
        double functionStep = experiment.getStep();
        double firstDerivative = (experiment.getValue(index) - experiment.getValue(index - 1)) / functionStep;
        if (firstDerivative >= 2.5) {
            double oscillationStartValue = experiment.getValue(index);
            Function oscillation = {};
            unsigned int indent = 50;
            for (unsigned int i = index - indent; i < index; i++) {
                oscillation.add(experiment.getElement(i));
            }
            while (index < experiment.size() && oscillationStartValue <= experiment.getValue(index)) {
                oscillation.add(experiment.getElement(index));
                index++;
            }
            if (index >= experiment.size()) {
                break;
            }
            unsigned int oscillationEnd = std::min(index + indent, experiment.size());
            for (; index < oscillationEnd; index++) {
                oscillation.add(experiment.getElement(index));
            }
            oscillations.append(oscillation);
        }
        index++;
    }
    return oscillations;
}

QVector<FunctionApproximation> ExperimentAnalyzer::calculateApproximations(const QVector<Function> &oscillations) const
{
    QVector<FunctionApproximation> oscillationsApproximation = {};
    std::array<double, 3> firstComponent = {0, 0, 0};
    for (const Function &oscillation : oscillations) {
        FunctionApproximation oscillationApproximation = approximator->approximate(oscillation, firstComponent);
        oscillationsApproximation.append(oscillationApproximation);
        QVector<double> coefficients = oscillationApproximation.getCoefficients();
        for (uint8_t i = 0; i < 3; i++) {
            firstComponent[i] = coefficients[i];
        }
    }
    return oscillationsApproximation;
}

ExperimentAnalysis ExperimentAnalyzer::formAnalysis(const QVector<FunctionApproximation> &oscillations) const
{
    ExperimentAnalysis analysis = {};
    for (const FunctionApproximation &oscillation : oscillations) {
        analysis.append(oscillation);
    }
    return analysis;
}
