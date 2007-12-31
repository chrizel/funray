#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include "vector.h"

#define WIDTH 800
#define HEIGHT 600

class Canvas : public QWidget
{
    Q_OBJECT

private:
    vec data[WIDTH][HEIGHT];

public:
    Canvas(QWidget *parent = 0);
    virtual ~Canvas();

    virtual void paintEvent(QPaintEvent *event);

public slots:
    void raytrace();
};

#endif
