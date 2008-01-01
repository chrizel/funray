#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <iostream>

struct vec {
public:
    double x, y, z;

    vec() : x(0), y(0), z(0) {};
    vec(double x, double y, double z) : x(x), y(y), z(z) {};

    double mag() { return sqrt(x*x + y*y + z*z); };
    void normal() {
        double mag = this->mag();
        if (mag > 0) {
            x /= mag;
            y /= mag;
            z /= mag;
        }
    };

    double dot(const vec &other) const {
        return (x * other.x)
             + (y * other.y)
             + (z * other.z);
    };

    void dump() {
        std::cout << "[" << x << "," << y << "," << z << "]" << std::endl;
    };

    const vec operator*(const double &s) {
        return vec(x * s, y * s, z * s);
    };
    
    const vec operator/(const double &s) {
        return vec(x / s, y / s, z / s);
    };

    const vec operator+(const vec &other) {
        return vec(x + other.x, y + other.y, z + other.z);
    };

    const vec operator-(const vec &other) {
        return vec(x - other.x, y - other.y, z - other.z);
    };

    const vec operator*(const vec &other) {
        return vec(x * other.x, y * other.y, z * other.z);
    };
};

#endif
