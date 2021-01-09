#include "functionelement.h"

bool FunctionElement::operator==(const FunctionElement &other) const {
    return ((x == other.x) && (f == other.f));
}

bool FunctionElement::operator!=(const FunctionElement &other) const {
    return ((x != other.x) || (f != other.f));
}
