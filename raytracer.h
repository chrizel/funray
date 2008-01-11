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

class Ray
{
public:
    Ray(const vec &pos, const vec &dir, const int px = 0, const int py = 0)
        : pos(pos), dir(dir.normal()), px(px), py(py) {};

    vec pos;
    vec dir;
    int px;
    int py;
};

class Light
{
public:
    Light(const vec &pos, const vec &color, const double power)
        : pos(pos), color(color), power(power) {};

    vec pos;
    vec color;
    double power;
};

class Primitive
{
public:
    vec color;
    double mirror;

    Primitive(const vec &color, double mirror = 0.5)
	: color(color), mirror(mirror) {};
    virtual ~Primitive() {};

    virtual double intercept(const Ray &ray) = 0;
    virtual const vec normalAt(vec &point) = 0;

    virtual const vec colorAt(vec & /* point */) {
        return color;
    };

    virtual double getMirror() { return mirror; };
    virtual void setMirror(double mirror) { this->mirror = mirror; };
};

class Sphere : public Primitive
{
public:
    Sphere(const vec &pos, double radius, const vec &color) 
        : Primitive(color), pos(pos), radius(radius) {};

    vec pos;
    double radius;

    virtual double intercept(const Ray &ray) {
      vec e = pos - ray.pos;
      double a = e.dot(ray.dir);
      double f = sqrt(radius * radius - (e.dot(e)) + (a * a));
      return a - f;
    };


    virtual const vec normalAt(vec &point) {
	/*
	vec v = (point - pos).normal(); 
	double r1 = 0.1 * (rand() % 100) / 100.0; 
	double r2 = 0.1 * (rand() % 100) / 100.0;
	double r3 = 0.1 * (rand() % 100) / 100.0;
	return (v + vec(r1, r2, r3)).normal();
	*/

	return (point - pos).normal();
    }
};

class Plane : public Primitive
{
public:
    Plane(const vec &pos, const vec &normal, const vec &color)
        : Primitive(color), pos(pos), normal(normal.normal()) {};

    vec pos;
    vec normal;

    virtual double intercept(const Ray &ray) {
        double d = pos.dot(normal);
        double a = d - ray.pos.dot(normal);
        double b = ray.dir.dot(normal);
        return a / b;
    };


    virtual const vec normalAt(vec & /* point */) {
        return this->normal;
    }

    virtual const vec colorAt(vec &point) {
        vec p = point + pos;
        int a = (int(fabs(p.z < 0 ? p.z - 1 : p.z)) % 2);
        int b = (int(fabs(p.x < 0 ? p.x - 1 : p.x)) % 2);
        return a == b ? vec(0.6, 0.6, 0.6) : vec(1, 1, 1);
    };
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
