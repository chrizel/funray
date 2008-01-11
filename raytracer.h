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

#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <cmath>
#include <vector>

#include "camera.h"
#include "vector.h"

class Primitive;
class Ray;

class Light
{
public:
    Light(const vec &pos, const vec &color, const double power)
        : pos(pos), color(color), power(power) {};

    vec pos;
    vec color;
    double power;
};


class RaytraceListener
{
public:
    virtual ~RaytraceListener() {};
    virtual void raytraceStart(Raytracer & /* raytracer */) {};
    virtual void raytraceEnd(Raytracer & /* raytracer */) {};
    virtual void raytraceLine(Raytracer & /* raytracer */, int /* line */) {};
};

class Raytracer
{
private:
    vec *pixels;
    int width;
    int height;

    RaytraceListener *listener;

    typedef std::vector<Primitive*> Prims;
    typedef Prims::iterator PrimsIterator;
    Prims prims;
    
    Light light;
    Camera camera;

    inline void setPixel(const int &x, const int &y, const vec &d) {
        pixels[width * y + x] = d.clamp();
    };
public:
    Raytracer(int width, int height);
    virtual ~Raytracer();

    void raytrace();
    void setListener(RaytraceListener *listener) { this->listener = listener; };

    void resetPixels();
    vec sendRay(Ray ray, int counter = 0);

    inline int getWidth() { return width; };
    inline int getHeight() { return height; };

    inline const vec getPixel(int x, int y) const {
        return pixels[width * y + x];
    };
};



#endif
