#include "mathfunctions.h"

// Вычисление результата скользящего среднего
Function getMA(const Function &function, const int &size) {
    Function result;
    // size - количество отсчетов интервала усреднения
    double sumx = 0; // сумма отсчетов на интервале
    std::vector<double> mas(size); // массив для хранения size отсчетов
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
    return result;
}

double findKeyByValue(const std::function<double (const double&)> &function, const double &value, const double &precision, const double &start) {
    double key = start;
    double step = 0.1;
    double step_left_value = INFINITY, in_key_value = INFINITY, step_right_value = INFINITY;
    double min = INFINITY;
    while (std::abs(function(key) - value) > precision) {
        step_left_value = std::abs(function(key - step) - value);
        in_key_value = std::abs(function(key) - value);
        step_right_value = std::abs(function(key + step) - value);
        min = std::min({step_left_value, in_key_value, step_right_value});
        if (min == step_left_value) {
            key -= step;
        } else if (min == in_key_value) {
            step /= 10.0;
        } else {
            key += step;
        }
    }
    return key;
}

double findMaximum(const std::function<double (const double&)> &function, const double &precision, const double &start) {
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

double findIntegral(const std::function<double (const double&)> &function, const double &start, const double &end, const double &precision) {
        double previous_integral = precision + 1, integral = 0;
        for (int N = 2; (N <= 4) || (std::abs(integral - previous_integral) > precision); N *= 2) {
            double step, sum2 = 0, sum4 = 0, sum = 0;
            step = (end - start) / (2 * N);
            for (int i = 1; i <= 2 * N - 1; i += 2) {
                sum4 += function(start + step * i);
                sum2 += function(start + step * (i + 1));
            }
            sum = function(start) + 4 * sum4 + 2 * sum2 - function(end);
            previous_integral = integral;
            integral = (step / 3) * sum;
        }
    return integral;
}
