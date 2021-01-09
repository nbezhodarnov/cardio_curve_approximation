#ifndef FUNCTIONELEMENT_H
#define FUNCTIONELEMENT_H

struct FunctionElement {
    double x;
    double f;

    bool operator==(const FunctionElement&) const;
    bool operator!=(const FunctionElement&) const;
};

#endif // FUNCTIONELEMENT_H
