#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <iostream>
using namespace std;

#include "vector.h"

class Camera
{
public:
    Camera(const vec &pos, const vec &dir, double hlen, double vlen)
        : pos(pos), dir(dir), hlen(hlen), vlen(vlen) {};

    vec pos;
    vec dir;
    double hlen;
    double vlen;
};

class Ray
{
public:
    Ray(const vec &pos, const vec &dir)
        : pos(pos), dir(dir.normal()) {};

    vec pos;
    vec dir;
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

    Primitive(const vec &color)
      : color(color), mirror(0.0) {};
    Primitive(const vec &color, double mirror)
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

class Raytracer;

class RaytraceListener
{
public:
    virtual ~RaytraceListener() {};
    virtual void raytraceStart(Raytracer & /* raytracer */) {};
    virtual void raytraceEnd(Raytracer & /* raytracer */) {};
    virtual void raytraceLine(Raytracer & /* raytracer */, int /* line */) {};
};

typedef struct _world {
  int prim_count;
  Primitive **prims;
  Light *light;
  Camera *camera;
} World;

class Raytracer
{
private:
    vec *pixels;
    int width;
    int height;
    RaytraceListener *listener;

    inline void setPixel(const int &x, const int &y, const vec &d) {
        pixels[width * y + x] = d.clamp();
    };

public:
    Raytracer(int width, int height);
    virtual ~Raytracer();

    void raytrace();
    void setListener(RaytraceListener *listener) { this->listener = listener; };

    void resetPixels();
    vec sendRay(World world, Ray ray);
    vec sendRay(World world, Ray ray, int counter);

    inline int getWidth() { return width; };
    inline int getHeight() { return height; };

    inline const vec getPixel(int x, int y) const {
        return pixels[width * y + x];
    };
};

#endif
