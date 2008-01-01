#include <algorithm>

#include "vector.h"
#include "raytracer.h"

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
  int prim_count = world.prim_count;
  Primitive **prims = world.prims;
  Light light = *(world.light);
  Camera camera = *(world.camera);

  Primitive *prim = 0;
  double length  = -1;
  for (int i = 0; i < prim_count; i++) {
    double len = prims[i]->intercept(ray);
    if ( (len > 0) && ((length < 0) || (len < length)) ) {
      prim = prims[i];
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
    for (int i = 0; i < prim_count; i++) {
      if (prims[i] == prim)
	continue;
      if (prims[i]->intercept(sray) > 0) {
                        hit = true;
                        break;
      }
    }

    if (!hit) {
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
      
      vec col = prim->colorAt(p)
	* light.color
	* i
	* ldexp(std::max(n.dot(h), 0.0), 3);
      
      return col;
    } else {
      return prim->colorAt(p) * vec(0.1, 0.1, 0.1);
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
    prims[1] = new Sphere(vec(3.0, 0.0, 8.0), 1.0, vec(1, 0, 0));
    prims[2] = new Sphere(vec(-3,  1, 7),  2, vec(0, 1, 0));
    prims[3] = new Sphere(vec(4,   0, 5),  1, vec(0, 0, 1));
    prims[4] = new Sphere(vec(-4,  0, 3),  1, vec(1, 1, 0));
    prims[5] = new Sphere(vec(1,  -.8, 2),  0.2, vec(1, 0, 1));
    //prims[6] = new Plane(vec(6, 0, 0), vec(-1, 0, 0), vec(0, 1, 1));

    Light light(vec(0.0, 8.0, -1.0), vec(0.2, 0.2, 0.2), 30.0);
    Camera camera(vec(0, 6, -8), vec(0, -1, 1), 1.333, 1.0);

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
