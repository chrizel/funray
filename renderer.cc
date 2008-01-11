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

#include <vector>

#include "renderer.h"
#include "vector.h"


Renderer::Renderer(const Scene &scene, int width, int height)
    : scene(scene), width(width), height(height), listener(0)
{
    pixels = new vec[width * height];
    resetPixels();
}

Renderer::~Renderer()
{
    delete [] pixels;
}

void Renderer::resetPixels()
{
    vec white(1, 1, 1);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            setPixel(x, y, white);
        }
    }
}

void Renderer::render()
{
    resetPixels();
    if (listener) listener->renderStart(*this);

    const int cycles = 4;
    for (int g = 0; g < cycles; g++) {
	for (int y = g; y < height; y += cycles) {
	    for (int x = 0; x < width; x++) {
		Ray ray(scene.camera.pos, scene.camera.dirVecFor(x, y, width, height));
		setPixel(x, y, scene.sendRay(ray));
	    }

	    if (listener) listener->renderLine(*this, y);
	}
    }


    if (listener) listener->renderEnd(*this);
}
