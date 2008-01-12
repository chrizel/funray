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

#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "camera.h"
#include "light.h"
#include "vector.h"
#include "dela.h"

class Primitive;
class Ray;

typedef std::vector<Primitive*> Prims;
typedef Prims::const_iterator PrimsIterator;

class Scene : public dela::Scriptable
{
public:
    Prims prims;
    Light *light;
    Camera *camera;

    Scene();
    virtual ~Scene();

    vec sendRay(Ray ray, int counter = 0) const;

    inline void addPrimitive(Primitive *p) { prims.push_back(p); };
    inline void setCamera(Camera *c) {
	if (camera) delete camera;
	camera = c;
    };

    inline void setLight(Light *l) {
	if (light) delete light;
	light = l;
    };
};

#endif
