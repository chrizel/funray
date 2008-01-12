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

#include <QDebug>
#include <QThread>

#include <iostream>
#include <vector>
#include <unistd.h>

#include "renderer.h"
#include "vector.h"


Worker::Worker(Renderer &renderer, int from, int step) 
    : renderer(renderer),
      from(from),
      step(step)
{
}

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

void Worker::run()
{
    std::cout << "Thread " << from << " started." << std::endl;

    const Scene &scene = renderer.getScene();
    int width = renderer.getWidth();
    int height = renderer.getHeight();

    for (int y = from; y < height; y += step) {
	for (int x = 0; x < width; x++) {
	    Ray ray(scene.camera->pos, scene.camera->dirVecFor(x, y, width, height));
	    renderer.setPixel(x, y, scene.sendRay(ray));
	}
    }

    std::cout << "Thread " << from << " finished." << std::endl;
}

void Renderer::render()
{
    if (!scene.camera) {
	qDebug() << "Renderer error: No camera defined!";
	exit(1);
    }

    resetPixels();
    if (listener) listener->renderStart(*this);

    const int thread_count = 3;

    Worker **workers = new Worker*[thread_count];

    for (int t = 0; t < thread_count; t++) {
	Worker *worker = new Worker(*this, t + 1, thread_count + 1);
	worker->start();
	workers[t] = worker;
    }

    for (int y = 0; y < height; y += thread_count + 1) {
	for (int x = 0; x < width; x++) {
	    Ray ray(scene.camera->pos, scene.camera->dirVecFor(x, y, width, height));
	    setPixel(x, y, scene.sendRay(ray));
	}
	if (listener) listener->renderLine(*this, y);
    }


    int count = thread_count;
    while (count) {
	for (int t = 0; t < thread_count; t++) {
	    if (workers[t] && workers[t]->isFinished()) {
		delete workers[t];
		workers[t] = 0;
		count--;
	    }
	}

	usleep(200);
    }

    if (listener) listener->renderEnd(*this);
}
