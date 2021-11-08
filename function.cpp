#include <QTextStream>

#include "function.h"

Function::Function() {
}

void Function::add(FunctionElement element) {
    elements.append(element);
}

void Function::add(double x, double f) {
    elements.append({x, f});
}


void Function::move(double x, double y) {
    for (unsigned int i = 0; i < (unsigned int)elements.size(); i++) {
        elements[i] = {elements[i].x + x, elements[i].f + y};
    }
}

void Function::moveByAxisX(double x) {
    move(x, 0);
}

void Function::moveByAxisY(double y) {
    move(0, y);
}


void Function::removeElementAt(unsigned int i) {
    elements.removeAt(i);
}


void Function::removeElementsFromBeginning(unsigned int count) {
    elements.remove(0, count);
}


void Function::removeElementsFromEnd(unsigned int count) {
    elements.remove(elements.size() - 1, count);
}

FunctionElement Function::getElement(unsigned int i) const {
    if (i < (unsigned int)elements.size()) {
        return elements[i];
    }
    QTextStream(stderr) << "Index is out of array!\n" << flush;
    return FunctionElement();
}

FunctionElement Function::getElement(int i) const {
    return getElement((unsigned int)i);
}

double Function::getKey(unsigned int i) const {
    if (i < (unsigned int)elements.size()) {
        return elements[i].x;
    }
    QTextStream(stderr) << "Index is out of array!\n" << flush;
    return double();
}

double Function::getKey(int i) const {
    return getKey((unsigned int)i);
}

double Function::getValue(unsigned int i) const {
    if (i < (unsigned int)elements.size()) {
        return elements[i].f;
    }
    QTextStream(stderr) << "Index is out of array!\n" << flush;
    return double();
}

double Function::getValue(int i) const {
    return getValue((unsigned int)i);
}

double Function::getValue(double x) const {
    for (int i = 0; i < elements.size(); i++) {
        if (x == elements[i].x) {
            return elements[i].f;
        }
    }
    QTextStream(stdout) << "The element hasn't been found!\n" << flush;
    return double();
}

double Function::getStep() const {
    double result = 0;
    for (int i = 1; i < elements.size(); i++) {
        result += elements[i].x - elements[i - 1].x;
    }
    result /= elements.size() - 1;
    return result;
}

unsigned int Function::size() const {
    return elements.size();
}

bool Function::operator==(const Function &other) const {
    return elements == other.elements;
}

bool Function::operator!=(const Function &other) const {
    return elements != other.elements;
}
