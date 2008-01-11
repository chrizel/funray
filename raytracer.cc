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

#include <algorithm>
#include <iostream> // DEBUG

#include "camera.h"
#include "primitives.h"
#include "raytracer.h"
#include "vector.h"

Raytracer::Raytracer(int width, int height)
    : width(width), height(height), listener(0),
      light(vec(0.0, 8.0, -1.0), vec(0.2, 0.2, 0.2), 30.0),
      camera(*this, vec(0, 0, -10), vec(0.0, 0.0, 1.0), 1.333, 1.0)

{
    pixels = new vec[width * height];
    resetPixels();

    prims.push_back(new Plane(vec(0, -1.0, 0), vec(0, 1, 0), vec(0, 1, 1)));
    prims.push_back(new Sphere(vec(0.0, 0.0, 3.0), 1.0, vec(1, 1, 1)));
    prims.push_back(new Sphere(vec(2.0, 0.0, 2.0), 1.0, vec(1, 1, 1)));
    prims.push_back(new Sphere(vec(4,   0, 5),  1, vec(0, 0, 1)));
    prims.push_back(new Sphere(vec(-4,  0, 3),  1, vec(1, 1, 0)));
    prims.push_back(new Sphere(vec(1,  -0.8, 2),  0.2, vec(1, 0, 1)));
}

Raytracer::~Raytracer()
{
    for (PrimsIterator it = prims.begin(); it != prims.end(); it++)
	delete *it;
}

void Raytracer::resetPixels()
{
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            setPixel(x, y, vec(1, 1, 1));
        }
    }
}

vec Raytracer::sendRay(Ray ray, int count)
{
    if (count > 100) {
	std::cout << "Infinity mirror..." << std::endl;
	return vec(0.0, 1.0, 1.0);
    }
  
    Primitive *prim = 0;
    double length  = -1;
    for (PrimsIterator it = prims.begin(); it != prims.end(); it++) {
	double len = (*it)->intercept(ray);
	if ( (len > 0.0001) && ((length < 0) || (len < length)) ) {
	    prim = *it;
	    length = len;
	}
    }
  
    if (prim) {
	// hit point in world coordinates
	vec p = (ray.dir * length) + ray.pos;
    
	// vector from hit point to light
	vec toLight = light.pos - p;
    
	// Cast ray from hit point to light source,
	// and check if object is between them...
	Ray sray(p, toLight);
	bool hit = false;
	for (PrimsIterator it = prims.begin(); it != prims.end(); it++) {
	    if (*it == prim)
		break;
	    if ((*it)->intercept(sray) > 0) {
		hit = true;
		break;
	    }
	}
    
	// normalized vector from hitpoint to viewer...
	vec v = (ray.dir * -1).normal();
    
	// normalized vector from hitpoint to light...
	vec l = light.pos - p;
	double len = l.mag(); // length needed for i below
	l = l.normal();
    
	// normal vector for hitpoint...
	vec n = prim->normalAt(p).normal();
    
	// halfway vector between view and light vector...
	vec h = (v + l).normal();
    
	double i = std::max(1.0 - (len / light.power), 0.0);
    
	vec col;
    
	if (hit)
	    col = vec(0,0,0);
	else
	    col = prim->colorAt(p)
		* light.color
		* i
		* ldexp(std::max(n.dot(h), 0.0), 3);
    
	n = n.normal();
    
	if (prim->getMirror() == 0.0) {
	    return col;
	}
	else {
	    double x = ray.dir.x;
	    double y = ray.dir.y;
	    double z = ray.dir.z;
      
	    ray.dir = ray.dir.normal();
      
	    vec mirrorRayTo((x*(1-2*n.x*n.x) + -2*y*n.x*n.y     + -2*z*n.x*n.z),
			    (-2*x*n.y*n.x +  y*(1-2*n.y*n.y) + -2*z*n.y*n.z),
			    (-2*x*n.z*n.x + -2*y*n.z*n.y    + z*(1-2*n.z*n.z)));
      
	    return 
		sendRay(Ray(p, mirrorRayTo), count+1)*prim->getMirror()+
		col*(1.0-prim->getMirror());
	}
    } else {
	// calculate world color...
	vec q = ray.dir * 100;
	double fac = q.y / 20.0;
	return vec(1.0 - (0.4 * fac), 1.0 - (0.2 * fac), 1.0);
    }
}

void Raytracer::raytrace()
{
    resetPixels();
    if (listener) listener->raytraceStart(*this);

    const int cycles = 4;
    for (int g = 0; g < cycles; g++) {
	for (int y = g; y < height; y += cycles) {
	    for (int x = 0; x < width; x++) {
		Ray ray(camera.pos, camera.dirVecFor(x, y), x, y);
		setPixel(x, y, sendRay(ray));
	    }

	    if (listener) listener->raytraceLine(*this, y);
	}
    }

    if (listener) listener->raytraceEnd(*this);
}
