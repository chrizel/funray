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
#include <QList>
#include <iostream> // DEBUG

#include "vector.h"
#include "raytracer.h"

using namespace std; // DEBUG

Raytracer::Raytracer(int width, int height)
    : width(width), height(height), listener(0)
{
    pixels = new vec[width * height];
    resetPixels();
}

Raytracer::~Raytracer()
{
    delete [] pixels;
}

void Raytracer::resetPixels()
{
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            setPixel(x, y, vec(1, 0, 1));
        }
    }
}



vec Raytracer::sendRay(const World world, Ray ray)
{
  return sendRay(world, ray, 0);
}

vec Raytracer::sendRay(const World world, Ray ray, int count)
{
  int prim_count = world.prim_count;
  Primitive **prims = world.prims;
  Light light = *(world.light);
  Camera camera = *(world.camera);
  //int use = -1;

  if (count > 100) {
    cout << "Infinity mirror..." << endl;
    return vec(0.0, 1.0, 1.0);
  }
  
  Primitive *prim = 0;
  double length  = -1;
  for (int i = 0; i < prim_count; i++) {
    /*if (i == dontUse)
      continue; */
    double len = prims[i]->intercept(ray);
    if ( (len > 0.0001) && ((length < 0) || (len < length)) ) {
      prim = prims[i];
      //  use = i;
      length = len;
    }
  }
  /*if (use == dontUse)
    prim = NULL; */
  
  if (prim) {
    // hit point in world coordinates
    vec p = (ray.dir * length) + ray.pos;
    
    // vector from hit point to light
    vec toLight = light.pos - p;
    
    // Cast ray from hit point to light source,
    // and check if object is between them...
    Ray sray(p, toLight);
    bool hit = false;
    for (int i = 0; i < prim_count; i++) {
      if (prims[i] == prim)
	continue;
      if (prims[i]->intercept(sray) > 0) {
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
      
      vec mirrorRayTo((x*(1-2*n.x*n.x) +   -2*y*n.x*n.y +     -2*z*n.x*n.z),
		      (-2*x*n.y*n.x +       y*(1-2*n.y*n.y) + -2*z*n.y*n.z),
		      (-2*x*n.z*n.x +       -2*y*n.z*n.y +     z*(1-2*n.z*n.z)));
      
      return 
	sendRay(world, Ray(p, mirrorRayTo), count+1)*prim->getMirror()+
	col*(1.0-prim->getMirror());
    }
  } else {
    return vec(0, 0, 0);
  }
}

void Raytracer::raytrace()
{
    const int prim_count = 6;

    Primitive *prims[prim_count];

    prims[0] = new Plane(vec(0, -1.0, 0), vec(0, 1, 0), vec(0, 1, 1));
    prims[0]->setMirror(.5);
    prims[1] = new Sphere(vec(3.0, 0.0, 8.0), 2.0, vec(1, 0, 0));
    prims[1]->setMirror(.8);
    prims[2] = new Sphere(vec(-3,  1, 7),  3, vec(0, 1, 0));
    prims[2]->setMirror(.99);
    prims[3] = new Sphere(vec(4,   0, 5),  2, vec(0, 0, 1));
    prims[3]->setMirror(.8);
    prims[4] = new Sphere(vec(-4,  0, 3),  2, vec(1, 1, 0));
    prims[4]->setMirror(.8);
    prims[5] = new Sphere(vec(1,  -.8, 2),  1, vec(1, 0, 1));
    prims[5]->setMirror(.8);
    //prims[6] = new Plane(vec(6, 0, 0), vec(-1, 0, 0), vec(0, 1, 1));

    Light light(vec(0.0, 8.0, -1.0), vec(0.2, 0.2, 0.2), 30.0);
    Camera camera(vec(0, 6, -8), vec(0, -1, 1), 1.333, 1.0);
    //Camera camera(vec(0, 1, -8), vec(0, 1, 1), 1.333, 1.0);
    

    World world;
    world.prim_count = prim_count;
    world.prims = prims;
    world.light = &light;
    world.camera = &camera;

    resetPixels();
    if (listener) listener->raytraceStart(*this);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Ray ray(camera.pos, 
                    vec(-0.666 + (1.333 / width) * x , 
                         0.0 - (1.0 / height) * y, 
                         1));
	    
	    setPixel(x, y, sendRay(world, ray));
        }

        if (listener) listener->raytraceLine(*this, y);
    }

    if (listener) listener->raytraceEnd(*this);

    for (int i = 0; i < prim_count; i++) {
        delete prims[i];
    }
}
