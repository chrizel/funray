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

#include <QGLWidget>
#include <QWidget>
#include <QFileSystemWatcher>

#include "scene.h"
#include "renderer.h"

class Canvas : public QGLWidget, public RendererListener
{
    Q_OBJECT

private:
    QFileSystemWatcher watcher;

    Scene *scene;
    Renderer *renderer;

    QString fileName;

public:
    Canvas(QWidget *parent = 0);
    virtual ~Canvas();

    //virtual void paintEvent(QPaintEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    virtual void renderStart(Renderer & /* renderer */) {};

    virtual void renderEnd(Renderer & /* renderer */) {
	//repaint();
	updateGL();
    };

    virtual void renderLine(Renderer &  renderer, int line) {
	//repaint(0, line, renderer.getWidth(), 1);
	updateGL();
    };

    bool loadScene(const QString &fileName);
    void setAutoRefresh(bool value);

public slots:
    void render();
    void save();
    void saveToFile(const QString &fileName);

private slots:
    void fileChanged(const QString &path);
};

#endif
