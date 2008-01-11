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

#ifndef RENDERER_H
#define RENDERER_H

#include <QList>
#include <QThread>

#include <cmath>

#include "scene.h"

class Renderer;

class RendererListener
{
public:
    virtual ~RendererListener() {};
    virtual void renderStart(Renderer & /* renderer */) {};
    virtual void renderEnd(Renderer & /* renderer */) {};
    virtual void renderLine(Renderer & /* renderer */, int /* line */) {};
};

class Worker : public QThread 
{
  private:
    Renderer &renderer;
    int from;
    int step;

  public:
    Worker(Renderer &renderer, int from, int step);
    void run();
};

class Renderer
{
private:
    const Scene &scene;
    int width;
    int height;
    RendererListener *listener;

    vec *pixels;
    void resetPixels();

public:


    Renderer(const Scene &scene, int width, int height);
    virtual ~Renderer();

    void render();

    inline const vec getPixel(int x, int y) const {
        return pixels[width * y + x];
    };
    inline void setPixel(const int &x, const int &y, const vec &d) {
        pixels[width * y + x] = d.clamp();
    };

    void setListener(RendererListener *listener) { 
	this->listener = listener; 
    };

    inline int getWidth() { 
	return width; 
    };
    inline int getHeight() { 
	return height; 
    };
    inline const Scene & getScene() {
	return scene;
    }
};

#endif
