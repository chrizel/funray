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

#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>

#include "canvas.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget window;

    Canvas *canvas = new Canvas();
    QPushButton *button = new QPushButton("Render");

    QObject::connect(button, SIGNAL(clicked()), canvas, SLOT(raytrace()));

    QVBoxLayout vbox;
    vbox.addWidget(button);
    vbox.addWidget(canvas);

    window.setLayout(&vbox);
    window.show();
    return app.exec();
}
