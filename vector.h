#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <cmath>
#include <iostream>

struct vec {
public:
    double x, y, z;

    vec() : x(0), y(0), z(0) {};
    vec(double x, double y, double z) : x(x), y(y), z(z) {};

    double mag() const { 
        return sqrt(x*x + y*y + z*z); 
    };

    const vec normal() const {
        double m = this->mag();
        return (m > 0) ? (*this / m) : *this;
    };

    double dot(const vec &other) const {
        return (x * other.x)
             + (y * other.y)
             + (z * other.z);
    };

    const vec clamp(const double &min = 0.0, const double &max = 1.0) const {
        return vec(std::max(min, std::min(max, x)),
                   std::max(min, std::min(max, y)),
                   std::max(min, std::min(max, z)));
    };

    const vec operator*(const double &s) const {
        return vec(x * s, y * s, z * s);
    };
    
    const vec operator/(const double &s) const {
        return vec(x / s, y / s, z / s);
    };

    const vec operator+(const vec &other) const { 
        return vec(x + other.x, y + other.y, z + other.z); 
    };

    const vec operator-(const vec &other) const {
        return vec(x - other.x, y - other.y, z - other.z);
    };

    const vec operator*(const vec &other) const {
        return vec(x * other.x, y * other.y, z * other.z);
    };

    void dump() {
        std::cout << "[" << x << "," << y << "," << z << "]" << std::endl;
    };

};

#endif
