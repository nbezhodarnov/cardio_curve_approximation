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
    void setConstant(const double &);
    QVector<double> getCoefficients() const;
    double getValue(const double&) const;
    double getFirstComponentValue(const double&) const;
    double getSecondComponentValue(const double&) const;
    double getFirstDerivativeValue(const double&) const;
    double getSecondDerivativeValue(const double&) const;
    double getConstant() const;

    bool operator==(const FunctionApproximation &other) const;
    bool operator!=(const FunctionApproximation &other) const;

private:
    QVector<double> coefficients;
};

#endif // FUNCTIONAPPROXIMATION_H
