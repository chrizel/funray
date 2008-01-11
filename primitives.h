#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <cmath>

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

#endif
