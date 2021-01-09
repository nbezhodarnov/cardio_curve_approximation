#ifndef FUNCTIONAPPROXIMATION_H
#define FUNCTIONAPPROXIMATION_H

#include <QVector>

class FunctionApproximation
{
public:
    FunctionApproximation(const double&, const double&, const double&, const double&, const double&, const double&, const double &constant = 0);
    FunctionApproximation(const QVector<double>&);
    void set(const double&, const double&, const double&, const double&, const double&, const double&, const double &constant = 0);
    void set(const QVector<double>&);
    QVector<double> getCoefficients();
    double getValue(const double&);
    double getFirstComponentValue(const double&);
    double getSecondComponentValue(const double&);
    double getFirstDerivativeValue(const double&);
    double getSecondDerivativeValue(const double&);

private:
    QVector<double> coefficients;
};

#endif // FUNCTIONAPPROXIMATION_H
