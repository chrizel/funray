#include <cmath>

#include "raytracer.h"
#include "vector.h"

Camera::Camera(Raytracer &rt, 
	       const vec &pos, 
	       const vec &dir, 
	       double hlen, 
	       double vlen)
    : rt(rt),
      pos(pos), 
      dir(dir.normal()), 
      hlen(hlen), 
      vlen(vlen) 
{
}

Camera::~Camera()
{
}

const vec Camera::dirVecFor(int x, int y) {
    vec v = vec(-(hlen / 2) + (hlen / rt.getWidth())  * x,
		(vlen / 2) - (vlen / rt.getHeight()) * y,
		1.333).normal();

    vec a = vec(0.0, 0.0, 1.0);
    vec b = dir;

    // angle between vector a and b:
    double ct = a.dot(b) / (a.mag() + b.mag());
    double st = sin(acos(ct));

    // calculate perpendicular vector from a and b
    vec n = xproduct(a, b).normal();

    // multiply vector v with rotation matrix
    vec r = vec( v.x * (n.x * n.x * (1 - ct) + ct) +
		 v.y * (n.x * n.y * (1 - ct) - n.z * st) +
		 v.z * (n.x * n.z * (1 - ct) + n.y * st),

		 v.x * (n.x * n.y * (1 - ct) + n.z * st) +
		 v.y * (n.y * n.y * (1 - ct) + ct) +
		 v.z * (n.y * n.z * (1 - ct) - n.x * st),

		 v.x * (n.x * n.z * (1 - ct) - n.y * st) +
		 v.y * (n.y * n.z * (1 - ct) + n.x * st) +
		 v.z * (n.z * n.z * (1 - ct) + ct) );

    return r;
};
