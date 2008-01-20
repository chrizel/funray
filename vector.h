/*funray - yet another raytracer
Copyright (C) 2008  Christian Zeller (chrizel@gmail.com) and
                    Simon Goller (neosam@gmail.com).

This program is free software; you can redistribute it and/or modify 
it under the terms of the GNU General Public License as published 
by the Free Software Foundation; either version 3 of the License, 
or (at your option) any later version.

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
General Public License for more details.

You should have received a copy of the GNU General Public License along 
with this program; if not, see <http://www.gnu.org/licenses/>. */

#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <cmath>
#include <iostream>

struct vec {
public:
    float x, y, z;

    vec() : x(0), y(0), z(0) {};
    vec(float x, float y, float z) : x(x), y(y), z(z) {};

    float mag() const { 
        return sqrt(x*x + y*y + z*z); 
    };

    const vec normal() const {
        float m = this->mag();
        return (m > 0) ? (*this / m) : *this;
    };

    float dot(const vec &other) const {
        return (x * other.x)
             + (y * other.y)
             + (z * other.z);
    };

    const vec clamp(const float &min = 0.0, const float &max = 1.0) const {
        return vec(std::max(min, std::min(max, x)),
                   std::max(min, std::min(max, y)),
                   std::max(min, std::min(max, z)));
    };

    const vec operator*(const float &s) const {
        return vec(x * s, y * s, z * s);
    };
    
    const vec operator/(const float &s) const {
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

inline vec xproduct(const vec &a, const vec &b)
{
    return vec(a.y * b.z - a.z * b.y,
	       a.z * b.x - a.x * b.z,
	       a.x * b.y - a.y * b.x);
}

class Ray
{
public:
    Ray(const vec &pos, const vec &dir)
        : pos(pos), dir(dir.normal()) {};

    vec pos;
    vec dir;
};

#endif
