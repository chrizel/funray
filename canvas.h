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
