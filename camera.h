#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

class Raytracer;

class Camera
{
private:
    Raytracer &rt;

public:
    Camera(Raytracer &rt, 
           const vec &pos, 
           const vec &dir, 
           double hlen, 
           double vlen);
    virtual ~Camera();

    vec pos;
    vec dir;
    double hlen;
    double vlen;

    const vec dirVecFor(int x, int y);
};

#endif
