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

#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

#include "raytracer.h"

class Canvas : public QWidget, public RaytraceListener
{
    Q_OBJECT

private:
    Raytracer raytracer;

public:
    Canvas(QWidget *parent = 0);
    virtual ~Canvas();

    virtual void paintEvent(QPaintEvent *event);

    virtual void raytraceStart(Raytracer & /* raytracer */) {
        repaint();
    };

    virtual void raytraceEnd(Raytracer & /* raytracer */) {
        repaint();
    };

    virtual void raytraceLine(Raytracer &  raytracer, int line) {
        repaint(0, line, raytracer.getWidth(), 1);
    };

public slots:
    void raytrace();
};

#endif
