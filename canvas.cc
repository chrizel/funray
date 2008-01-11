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

#include <iostream>

#include <QApplication>
#include <QFileDialog>
#include <QPainter>
#include <QPaintEvent>
#include <QTime>
#include <QWidget>

#include "canvas.h"
#include "vector.h"
#include "renderer.h"

static void drawRendering(QPainter &painter, Renderer &renderer,
			  int y1, int y2, int x1, int x2)
{
    for (int y = y1; y < y2; y++) {
        for (int x = x1; x < x2; x++) {
            const vec & pixel = renderer.getPixel(x, y);
            painter.setPen(QColor((int)(pixel.x * 255), 
                                  (int)(pixel.y * 255), 
                                  (int)(pixel.z * 255)));
            painter.drawPoint(x, y);
        }
    }
}

Canvas::Canvas(QWidget *parent)
    : QWidget(parent),
      scene(),
      renderer(scene, 640, 480)
{
    renderer.setListener(this);
    setMinimumSize(renderer.getWidth(), renderer.getHeight());
    setMaximumSize(renderer.getWidth(), renderer.getHeight());
}

Canvas::~Canvas()
{
}

void Canvas::render()
{
    std::cout << "Start..." << std::endl;
    QTime t;
    t.start();
    renderer.render();
    std::cout << "Finished in " << t.elapsed() << " ms." << std::endl;
}

void Canvas::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
						    "", tr("Images (*.png)"));
    if (!fileName.isEmpty()) {
	QImage image( renderer.getWidth(), renderer.getHeight(), QImage::Format_RGB32 );
	QPainter painter(&image);
	drawRendering(painter, renderer, 0, renderer.getHeight(), 0, renderer.getWidth());
	image.save(fileName, "png");
    }
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rect = event->rect();
    int y1 = rect.y(),
        y2 = y1 + rect.height(),
        x1 = rect.x(),
        x2 = x1 + rect.width();
    drawRendering(painter, renderer, y1, y2, x1, x2);
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Q)
	close();
    else if (event->key() == Qt::Key_S)
	save();
    else if ((event->key() == Qt::Key_R)
	     || (event->key() == Qt::Key_F5)
	     || (event->key() == Qt::Key_Return))
	render();
}
