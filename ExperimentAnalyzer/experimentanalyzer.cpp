#include "experimentanalyzer.h"

ExperimentAnalyzer::ExperimentAnalyzer(std::unique_ptr<AbstractFunctionApproximator> functionApproximator) :
    approximator(std::move(functionApproximator))
{

}

ExperimentAnalysis ExperimentAnalyzer::analyze(const Function &experiment) const
{
    QVector<Function> oscillations = extractOscillations(experiment);
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
    double derivativeGate = 2.5;
    QVector<Function> oscillations = {};
    int index = 0;
    while (index + 1 < static_cast<int>(experiment.size())) {
        index++;
        double functionStep = experiment.getStep();
        double firstDerivative = (experiment.getValue(index) - experiment.getValue(index - 1)) / functionStep;
        if (firstDerivative >= derivativeGate) {
            double firstDerivativesSum = 0;
            for (unsigned int i = index + 1; i < std::min(experiment.size(), static_cast<unsigned int>(index + 6)); i++) {
                firstDerivativesSum += (experiment.getValue(i) - experiment.getValue(i - 1)) / functionStep;
            }
            if (firstDerivativesSum < 3 * firstDerivative) {
                derivativeGate = firstDerivative;
                continue;
            }

            double oscillationStartValue = experiment.getValue(index);
            Function oscillation = {};
            int indent = 0;
            int startIndex = index;
            while (index < static_cast<int>(experiment.size()) && oscillationStartValue <= experiment.getValue(index)) {
                index++;
            }
            if (index >= static_cast<int>(experiment.size())) {
                break;
            }
            if (index - startIndex < 5) {
                continue;
            }
            indent = (index - startIndex) * 15 / 100;
            int oscillationEnd = std::min(index + indent, static_cast<int>(experiment.size()));
            for (index = std::max(startIndex - indent, 0); index < oscillationEnd; index++) {
                oscillation.add(experiment.getElement(index));
            }
            oscillations.append(oscillation);
        }
    }
    return oscillations;
}

QVector<FunctionApproximation> ExperimentAnalyzer::calculateApproximations(const QVector<Function> &oscillations) const
{
    QVector<FunctionApproximation> oscillationsApproximation = {};
    FunctionApproximation firstOscillationApproximation(0, 0, 0, 0, 0, 0);
    bool isFirstOscillationApproximationSet = false;
    std::array<double, 3> firstComponent = {0, 0, 0};
    QVector<double> coefficients = {0, 0, 0, 0, 0, 0, 0};
    for (const Function &oscillation : oscillations) {
        coefficients[2] += oscillation.getKey(0);
        coefficients[5] += oscillation.getKey(0);
        for (uint8_t i = 0; i < 3; i++) {
            firstComponent[i] = coefficients[i];
        }
        firstOscillationApproximation = FunctionApproximation(coefficients);
        firstComponent = {0, 0, 0};
        FunctionApproximation oscillationApproximation = approximator->approximate(oscillation, firstOscillationApproximation, firstComponent);
        if (!isFirstOscillationApproximationSet) {
            firstOscillationApproximation = oscillationApproximation;
            coefficients = oscillationApproximation.getCoefficients();
            isFirstOscillationApproximationSet = true;
        }
        oscillationsApproximation.append(oscillationApproximation);
        coefficients[2] -= oscillation.getKey(0);
        coefficients[5] -= oscillation.getKey(0);
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
