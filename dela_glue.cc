/*funray - yet another raytracer
opyright (C) 2008  Christian Zeller (chrizel@gmail.com) and
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

#include <QByteArray>
#include <QDebug>

#include "dela.h"
#include "dela_builtins.h"
#include "dela_glue.h"

#include "camera.h"
#include "light.h"
#include "primitives.h"
#include "scene.h"

static Scene *curScene = 0;

static dela::Scriptable* scene(dela::Engine *e, dela::List *params)
{
    Scene *lastScene = curScene;
    curScene = new Scene();
    for (dela::List::iterator it = params->begin(); it != params->end(); it++)
	e->eval(*it);

    std::swap(curScene, lastScene);
    return lastScene;
}

static dela::Scriptable* sphere(dela::Engine *e, dela::List *params)
{
    if (!curScene) {
	qDebug() << "dela_glue error: No curScene set";
	exit(1);
    }

    vec pos = vec(e->readNumberProp(params, "position", 0),
		  e->readNumberProp(params, "position", 1),
		  e->readNumberProp(params, "position", 2));

    double radius = e->readNumberPropDef(params, "radius", 0, 1);

    vec color = vec(e->readNumberPropDef(params, "color", 0, 1),
		    e->readNumberPropDef(params, "color", 1, 1),
		    e->readNumberPropDef(params, "color", 2, 1));

    Sphere *sphere = new Sphere(pos, radius, color);
    curScene->addPrimitive(sphere);
    return sphere;
}

static dela::Scriptable* plane(dela::Engine *e, dela::List *params)
{
    if (!curScene) {
	qDebug() << "dela_glue error: No curScene set";
	exit(1);
    }

    vec pos = vec(e->readNumberPropDef(params, "position", 0, 0),
		  e->readNumberPropDef(params, "position", 1, -1),
		  e->readNumberPropDef(params, "position", 2, 0));

    vec normal = vec(e->readNumberPropDef(params, "normal", 0, 0),
		     e->readNumberPropDef(params, "normal", 1, 1),
		     e->readNumberPropDef(params, "normal", 2, 0));

    vec color = vec(e->readNumberPropDef(params, "color", 0, 1),
		    e->readNumberPropDef(params, "color", 1, 1),
		    e->readNumberPropDef(params, "color", 2, 1));

    Plane *plane = new Plane(pos, normal, color);
    curScene->addPrimitive(plane);
    return plane;
}

static dela::Scriptable* camera(dela::Engine *e, dela::List *params)
{
    if (!curScene) {
	qDebug() << "dela_glue error: No curScene set";
	exit(1);
    }

    vec pos = vec(e->readNumberPropDef(params, "position", 0, 0),
		  e->readNumberPropDef(params, "position", 1, 0),
		  e->readNumberPropDef(params, "position", 2, -10));

    vec dir = vec(e->readNumberPropDef(params, "direction", 0, 0),
		  e->readNumberPropDef(params, "direction", 1, 0),
		  e->readNumberPropDef(params, "direction", 2, 1));

    vec up = vec(e->readNumberPropDef(params, "up", 0, 0),
		 e->readNumberPropDef(params, "up", 1, 1),
		 e->readNumberPropDef(params, "up", 2, 0));


    double hlen = e->readNumberPropDef(params, "hlen", 0, 1.333);
    double vlen = e->readNumberPropDef(params, "vlen", 0, 1.0);

    Camera *camera = new Camera(pos, dir, up, hlen, vlen);
    curScene->setCamera(camera);
    return camera;
}


static dela::Scriptable* light(dela::Engine *e, dela::List *params)
{
    if (!curScene) {
	qDebug() << "dela_glue error: No curScene set";
	exit(1);
    }

    vec pos = vec(e->readNumberPropDef(params, "position", 0, 0),
		  e->readNumberPropDef(params, "position", 1, 8),
		  e->readNumberPropDef(params, "position", 2, -1));

    vec color = vec(e->readNumberPropDef(params, "color", 0, .2),
		    e->readNumberPropDef(params, "color", 1, .2),
		    e->readNumberPropDef(params, "color", 2, .2));

    double power = e->readNumberPropDef(params, "power", 0, 30.0);

    Light *light = new Light(pos, color, power);
    curScene->setLight(light);
    return light;
}


void addDelaGlue(dela::Engine *e)
{
    e->addMacro("scene",  &scene);
    e->addMacro("sphere", &sphere);
    e->addMacro("plane",  &plane);
    e->addMacro("camera", &camera);
    e->addMacro("light",  &light);
}
