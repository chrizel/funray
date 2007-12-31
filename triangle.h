#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "math.h"

class Triangle
{
private:
    vec a;
    vec b;
    vec c;

public:
    Triangle(vec a, vec b, vec c) : a(a), b(b), c(c) {};
};

#endif
