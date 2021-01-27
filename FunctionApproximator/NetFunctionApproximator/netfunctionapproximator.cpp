#include <math.h>
#include <QMessageBox>
#include <QTextStream>
#include <QApplication>

#include "netfunctionapproximator.h"

inline LineIndexes::LineIndexes(unsigned int begin_input, unsigned int end_input) {
    begin = begin_input;
    end = end_input;
}

inline bool LineIndexes::isIndexInLine(unsigned int index) {
    return ((index >= begin) && (index <= end));
}

inline bool LineIndexes::isIndexOutOfLine(unsigned int index) {
    return ((index < begin) || (index > end));
}

NetFunctionApproximator::NetFunctionApproximator(QWidget *ptr): AbstractFunctionApproximator(ptr) {
}

FunctionApproximation NetFunctionApproximator::approximate(const Function &function) {
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(QString::fromUtf8("Пожалуйста, подождите"));
    msgBox.setText(QString::fromUtf8("Пожалуйста, подождите. Идут вычисления."));
    msgBox.setStandardButtons(QMessageBox::NoButton);
    msgBox.setModal(true);
    msgBox.show();
    QApplication::processEvents();

    QTextStream out(stdout);
    unsigned int min_local = 0, n = function.size();
    for (unsigned int i = 1; i < n; ++i) {
        if (function.getValue(i) < function.getValue(min_local)) {
            min_local = i;
        }
    }
    double coefficient = function.getValue(min_local);
    Function funcTemp;
    for (unsigned int i = 0; i < n; ++i) {
        funcTemp.add({function.getKey(i), function.getValue(i) - coefficient});
    }

    unsigned int index = n / 10, peak_beginning = 0;
    bool is_peak_unavailable = false;
    LineIndexes peakLine1, peakLine2;
    double a1, a2, b1, b2, c1, c2, step = function.getStep(), peak_length = 0;
    while (funcTemp.getValue(index) <= funcTemp.getValue(index + 1)) {
        if (function.getValue(index) == funcTemp.getValue(index + 1)) {
            peak_length += function.getKey(index + 1) - function.getKey(index);
            if (peak_length > 0.01) {
                is_peak_unavailable = true;
            }
        } else if (function.getValue(index + 1) == funcTemp.getValue(index + 2)) {
            peak_beginning = index + 1;
        }
        index++;
        if ((function.getValue(index + 1) > function.getValue(index + 2)) && (is_peak_unavailable)) {
            peakLine1 = {peak_beginning, index + 1};
            index -= (index + 1 - peak_beginning) / 2 - 1;
            break;
        }
    }
    c1 = funcTemp.getKey(index);
    a1 = funcTemp.getValue(index);
    b1 = std::abs(-log(funcTemp.getValue(index * 100 / 245) / a1) / (pow(funcTemp.getKey(index * 100 / 245) - c1, 2)));

    if (is_peak_unavailable) {
        a1 += coefficient / 2;
    }

    index = int(n * 0.9);
    is_peak_unavailable = false;
    peak_length = 0;
    while (funcTemp.getValue(index) <= funcTemp.getValue(index - 1)) {
        if (function.getValue(index) == funcTemp.getValue(index - 1)) {
            peak_length += function.getKey(index) - function.getKey(index - 1);
            if (peak_length > 0.01) {
                is_peak_unavailable = true;
            }
        } else if (function.getValue(index - 1) == funcTemp.getValue(index - 2)) {
            peak_beginning = index - 1;
        }
        index--;
        if ((function.getValue(index - 1) > function.getValue(index - 2)) && (is_peak_unavailable)) {
            peakLine2 = {index - 1, peak_beginning};
            index += (peak_beginning - index + 1) / 2 - 1;
            break;
        }
    }
    c2 = funcTemp.getKey(index);
    a2 = funcTemp.getValue(index);
    b2 = std::abs(-log(funcTemp.getValue(index + (n - index) * 100 / 182) / a2) / (pow(funcTemp.getKey(index + (n - index) * 100 / 182) - c2, 2)));

    bool long_line = false;
    if ((c1 > c2) && (!is_peak_unavailable)) {
        long_line = true;
        int max_local = a1;
        index = n / 10;
        while ((max_local - funcTemp.getValue(index) > 4) || (funcTemp.getValue(index) < funcTemp.getValue(index + 1))) {
            index++;
        }
        c1 = funcTemp.getKey(index);

        index = int(n * 0.9);
        while ((max_local - funcTemp.getValue(index) > 4) || (funcTemp.getValue(index) < funcTemp.getValue(index - 1))) {
            index--;
        }
        c2 = funcTemp.getKey(index);
    }

    if (is_peak_unavailable) {
        a2 += coefficient / 2;
    }

    bool are_two_extremums = true;
    if (std::abs(c1 - c2) <= step) {
        unsigned int index_temp = n;
        while (funcTemp.getValue(index_temp - 1) < funcTemp.getValue(index_temp)) {
            index_temp--;
        }
        while ((funcTemp.getValue(index_temp) <= funcTemp.getValue(index_temp - 1)) && (index_temp > (unsigned int)(n * 0.9))) {
            index_temp--;
        }
        if (index_temp <= n * 0.9) {
            index_temp = 0;
            while(funcTemp.getValue(index_temp + 1) < funcTemp.getValue(index_temp)) {
                index_temp++;
            }
            while ((funcTemp.getValue(index_temp) <= funcTemp.getValue(index_temp + 1)) && (index_temp < n / 10)) {
                index_temp++;
            }
            if (index_temp >= n / 10) {
                are_two_extremums = false;
            } else {
                c1 = funcTemp.getKey(index_temp);
                a1 = funcTemp.getValue(index_temp);
                b1 = std::abs(-log(funcTemp.getValue(index_temp / 2) / a1) / (pow(funcTemp.getKey(index_temp / 2) - c1, 2)));
            }
        } else {
            c2 = funcTemp.getKey(index_temp);
            a2 = funcTemp.getValue(index_temp);
            b2 = std::abs(-log(funcTemp.getValue(index_temp + (n - index_temp) / 2) / a2) / (pow(funcTemp.getKey(index_temp + (n - index_temp) / 2) - c2, 2)));
        }
    }


    out << "a1 = " << a1 << ", b1 = " << b1 << ", c1 = " << c1 << '\n' << Qt::flush;
    if (are_two_extremums) {
        out << "a2 = " << a2 << ", b2 = " << b2 << ", c2 = " << c2 << "\n\n" << Qt::flush;
    } else {
        a1 /= 2;
        a2 /= 2;
        out << '\n';
    }

    double step1 = 0.5, step2 = 0.01, step3 = 0.1, min_value = INFINITY, previous_value, temp;
    if ((a1 > 1000) || (a2 > 1000)) {
        step1 = 2;
        step3 = 0.05;
    } else if ((a1 > 100) || (a2 > 100)) {
        step1 = 1;
        step3 = 0.1;
    } else if ((a1 > 10) || (a2 > 10)) {
        step1 = 0.5;
        step3 = 0.5;
    } else {
        step1 = 0.1;
        step3 = 1;
    }
    if ((b1 > 10) || (b2 > 10)) {
        step2 = 0.5;
    } else if ((b1 > 1) || (b2 > 1)) {
        step2 = 0.05;
    } else {
        step2 = 0.01;
    }
    if (!are_two_extremums) {
        step1 *= 2;
        step2 *= 2;
        step3 *= 2;
    }
    unsigned int numbers_of_points = n, counter = 0;
    if (n < numbers_of_points) {
        numbers_of_points = n;
    }
    if ((n > 150) && (!long_line)) {
        numbers_of_points = 50;
    } else if (n > 150) {
        numbers_of_points = 100;
    }
    uint8_t min_point_indexes[6];
    for (uint8_t i = 0; i < 6; ++i) {
        min_point_indexes[i] = 0;
    }
    bool q = true, need_center_correction = false;
    uint8_t center_correcter = 1;
    int little_changes_count = 0;
    long double reset[6];
    while (min_value > 18) {
        q = false;
        previous_value = min_value;
        for (uint8_t i = 0; i < 5; ++i) {
            for (uint8_t j = 0; j < 5; ++j) {
                for (uint8_t k = 0; k < 5; ++k) {
                    for (uint8_t l = 0; l < 5; ++l) {
                        for (uint8_t m = 0; m < 5; ++m) {
                            for (uint8_t r = 0; r < 5; ++r) {
                                QApplication::processEvents();
                                temp = 0;
                                for (unsigned int p = 0; p < numbers_of_points; ++p) {
                                    center_correcter = 1;
                                    if ((p > n * 0.25) && (p < n * 0.75)) {
                                        if (need_center_correction) {
                                            center_correcter = 8;
                                        }
                                    }
                                    long double approximating_function = - std::abs(a1 - (2 - i) * step1) * exp(-std::abs(b1 - (2 - j) * step2) * pow(funcTemp.getKey(p * n / numbers_of_points) - c1 + (2 - k) * step3, 2)) - std::abs(a2 - (2 - l) * step1) * exp(-std::abs(b2 - (2 - m) * step2) * pow(funcTemp.getKey(p * n / numbers_of_points) - c2 + (2 - r) * step3, 2));
                                    if (((peakLine1.isIndexOutOfLine(p * n / numbers_of_points)) && (peakLine2.isIndexOutOfLine(p * n / numbers_of_points))) || (function.getValue(p * n / numbers_of_points) + approximating_function > 0)) {
                                        temp += center_correcter * std::abs(funcTemp.getValue(p * n / numbers_of_points) + approximating_function);
                                    }
                                }
                                if (temp < min_value) {
                                    q = true;
                                    min_value = temp;
                                    min_point_indexes[0] = i;
                                    min_point_indexes[1] = j;
                                    min_point_indexes[2] = k;
                                    min_point_indexes[3] = l;
                                    min_point_indexes[4] = m;
                                    min_point_indexes[5] = r;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (!q) {
            step1 /= 10;
            step2 /= 10;
            step3 /= 10;
            if (step1 < 1e-20) {
                step1 = 0.5;
                step2 = 0.01;
                step3 = 0.1;

                if (((min_value > 160) && (!long_line)) || ((long_line) && (min_value > 200))) {
                    temp = min_value + 301;
                    reset[0] = a1;
                    reset[1] = b1;
                    reset[2] = c1;
                    reset[3] = a2;
                    reset[4] = b2;
                    reset[5] = c2;
                    while (temp - min_value > 300) {
                        a1 = reset[0] + (rand() % 100 - 50) / 1.0;
                        b1 = reset[1] + (rand() % 10 - 5) / 10.0;
                        c1 = reset[2] + (rand() % 10 - 5) / 1.0;
                        a2 = reset[3] + (rand() % 100 - 50) / 1.0;
                        b2 = reset[4] + (rand() % 10 - 5) / 10.0;
                        c2 = reset[5] + (rand() % 10 - 5) / 1.0;

                        temp = 0;
                        for (unsigned int p = 0; p < numbers_of_points; ++p) {
                            center_correcter = 1;
                            if ((p > n * 0.25) && (p < n * 0.75)) {
                                if (need_center_correction) {
                                    center_correcter = 8;
                                }
                            }
                            if ((peakLine1.isIndexOutOfLine(p * n / numbers_of_points)) && (peakLine2.isIndexOutOfLine(p * n / numbers_of_points))) {
                                temp += center_correcter * std::abs(funcTemp.getValue(p * n / numbers_of_points) - std::abs(a1) * exp(-std::abs(b1) * pow(funcTemp.getKey(p * n / numbers_of_points) - c1, 2)) - std::abs(a2) * exp(-std::abs(b2) * pow(funcTemp.getKey(p * n / numbers_of_points) - c2, 2)));
                            }
                        }
                    }
                }

                min_value = temp;
                counter = 0;
            }
            continue;
        }
        a1 -= (2 - min_point_indexes[0]) * step1;
        b1 -= (2 - min_point_indexes[1]) * step2;
        c1 -= (2 - min_point_indexes[2]) * step3;
        a2 -= (2 - min_point_indexes[3]) * step1;
        b2 -= (2 - min_point_indexes[4]) * step2;
        c2 -= (2 - min_point_indexes[5]) * step3;
        if ((std::abs(previous_value - min_value) < 0.0001) && (((min_value < 160) && (!long_line)) || ((long_line) && (min_value < 200)))) {
            little_changes_count++;
            if (little_changes_count >= 10) {
                break;
            }
        } else {
            little_changes_count = 0;
        }
        if ((min_point_indexes[0] % 4 != 0) && (min_point_indexes[1] % 4 != 0) && (min_point_indexes[2] % 4 != 0) && (min_point_indexes[3] % 4 != 0) && (min_point_indexes[4] % 4 != 0) && (min_point_indexes[5] % 4 != 0)) {
            step1 /= 10;
            step2 /= 10;
            step3 /= 10;
            //counter = 0;
        } else {
            counter++;
            if ((counter % 10 == 0) && (step2 < 0.01)) {
                step1 = 0.5 * (counter / 10);
                step2 = 0.01 * (counter / 10);
                step3 = 0.1 * (counter / 10);
                //counter = 0;
            }
            if ((counter % 100 == 0) && (min_value > 160)) {
                step1 = 0.5;
                step2 = 0.01;
                step3 = 0.1;

                temp = min_value + 301;
                reset[0] = a1;
                reset[1] = b1;
                reset[2] = c1;
                reset[3] = a2;
                reset[4] = b2;
                reset[5] = c2;
                while (temp - min_value > 300) {
                    a1 = reset[0] + (rand() % 100 - 50) / 1.0;
                    b1 = reset[1] + (rand() % 10 - 5) / 10.0;
                    c1 = reset[2] + (rand() % 10 - 5) / 1.0;
                    a2 = reset[3] + (rand() % 100 - 50) / 1.0;
                    b2 = reset[4] + (rand() % 10 - 5) / 10.0;
                    c2 = reset[5] + (rand() % 10 - 5) / 1.0;

                    temp = 0;
                    for (unsigned int p = 0; p < numbers_of_points; ++p) {
                        center_correcter = 1;
                        if ((p > n * 0.25) && (p < n * 0.75)) {
                            if (need_center_correction) {
                                center_correcter = 8;
                            }
                        }
                        if ((peakLine1.isIndexOutOfLine(p * n / numbers_of_points)) && (peakLine2.isIndexOutOfLine(p * n / numbers_of_points))) {
                            temp += center_correcter * std::abs(funcTemp.getValue(p * n / numbers_of_points) - std::abs(a1) * exp(-std::abs(b1) * pow(funcTemp.getKey(p * n / numbers_of_points) - c1, 2)) - std::abs(a2) * exp(-std::abs(b2) * pow(funcTemp.getKey(p * n / numbers_of_points) - c2, 2)));
                        }
                    }
                }

                min_value = temp;
                counter = 0;
            }
        }
        temp = 0;
        if (!need_center_correction) {
            for (unsigned int p = 0; p < numbers_of_points; ++p) {
                center_correcter = 0;
                if ((p > n * 0.25) && (p < n * 0.75)) {
                    center_correcter = 1;
                }
                if ((peakLine1.isIndexOutOfLine(p * n / numbers_of_points)) && (peakLine2.isIndexOutOfLine(p * n / numbers_of_points))) {
                    temp += center_correcter * std::abs(funcTemp.getValue(p * n / numbers_of_points) - std::abs(a1) * exp(-std::abs(b1) * pow(funcTemp.getKey(p * n / numbers_of_points) - c1, 2)) - std::abs(a2) * exp(-std::abs(b2) * pow(funcTemp.getKey(p * n / numbers_of_points) - c2, 2)));
                }
            }
            if ((min_value < 80) && (temp > 50)) {
                need_center_correction = true;
                min_value = INFINITY;
            }
        }
        out << "a1 = " << a1 << ", b1 = " << b1 << ", c1 = " << c1 << '\n' << Qt::flush;
        out << "a2 = " << a2 << ", b2 = " << b2 << ", c2 = " << c2 << '\n' << Qt::flush;
        out << min_value << ' ' << previous_value << '\n' << Qt::flush;

        for (uint8_t i = 0; i < 6; ++i) {
            out << (int)min_point_indexes[i] << ' ' << Qt::flush;
        }
        out << '\n' << step1 << ' ' << step2 << ' ' << step3 << '\n' << Qt::flush;
        out << "\n\n" << Qt::flush;
    }

    a1 = std::abs(a1);
    a2 = std::abs(a2);
    b1 = std::abs(b1);
    b2 = std::abs(b2);


    out << "a1 = " << a1 << ", b1 = " << b1 << ", c1 = " << c1 << '\n' << Qt::flush;
    out << "a2 = " << a2 << ", b2 = " << b2 << ", c2 = " << c2 << '\n' << Qt::flush;

    return FunctionApproximation(a1, b1, c1, a2, b2, c2, coefficient);
}
