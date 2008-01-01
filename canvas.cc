#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#include "canvas.h"
#include "vector.h"

template <class T>
T max(T a, T b) { return a > b ? a : b; }

template <class T>
T min(T a, T b) { return a < b ? a : b; }

class Ray
{
public:
    Ray(const vec &pos, const vec &dir)
        : pos(pos), dir(dir) 
    {
        this->dir.normal();
    };

    vec pos;
    vec dir;
};

class Light
{
public:
    Light(const vec &pos, const double power)
        : pos(pos), power(power) {};
    vec pos;
    double power;
};

class Primitive
{
public:
    vec color;

    Primitive(const vec &color)
        : color(color) {};
    virtual ~Primitive() {};

    virtual double intercept(const Ray &ray) = 0;
    virtual const vec normalAt(vec &point) = 0;

    virtual const vec colorAt(vec & /* point */) {
        return color;
    };
};

class Sphere : public Primitive
{
public:
    Sphere(const vec &pos, double radius, const vec &color) 
        : Primitive(color), pos(pos), radius(radius) {};

    vec pos;
    double radius;

    virtual double intercept(const Ray &ray) {
        vec e = pos - ray.pos;
        double a = e.dot(ray.dir);
        double f = sqrt(radius * radius - (e.dot(e)) + (a * a));
        return a - f;
    };


    virtual const vec normalAt(vec &point) {
        vec n = point - pos;
        n.normal();
        return n;
    }
};

class Plane : public Primitive
{
public:
    Plane(const vec &pos, const vec &normal, const vec &color)
        : Primitive(color), pos(pos), normal(normal) {
        this->normal.normal();
    };

    vec pos;
    vec normal;

    virtual double intercept(const Ray &ray) {
        double d = pos.dot(normal);
        double a = d - ray.pos.dot(normal);
        double b = ray.dir.dot(normal);
        return a / b;
    };


    virtual const vec normalAt(vec & /* point */) {
        return this->normal;
    }

    virtual const vec colorAt(vec &point) {
        int a = (int(fabs(point.z < 0 ? point.z - 1 : point.z)) % 2);
        int b = (int(fabs(point.x < 0 ? point.x - 1 : point.x)) % 2);
        if (a == b) {
            return vec(0.6, 0.6, 0.6);
        } else {
            return vec(1, 1, 1);
        }
    };
};

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(WIDTH, HEIGHT);
    setMaximumSize(WIDTH, HEIGHT);
}

Canvas::~Canvas()
{
}

void Canvas::raytrace()
{
    std::cout << "Start..." << std::endl;

    const int prim_count = 6;

    Primitive *prims[prim_count];

    prims[0] = new Plane(vec(0, -1.0, 0), vec(0, 1, 0), vec(0, 1, 1));
    prims[1] = new Sphere(vec(3.0, 0.0, 8.0), 1.0, vec(1, 0, 0));
    prims[2] = new Sphere(vec(-3,  1, 7),  2, vec(0, 1, 0));
    prims[3] = new Sphere(vec(4,   0, 5),  1, vec(0, 0, 1));
    prims[4] = new Sphere(vec(-4,  0, 3),  1, vec(1, 1, 0));
    prims[5] = new Sphere(vec(1,  -.8, 2),  0.2, vec(1, 0, 1));

    Light light(vec(0.0, 8.0, -1.0), 15.0);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            data[x][y] = vec(1, 0, 1);
        }
    }
    repaint();

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            /*
            Ray ray(vec(0, 0, 0), 
                    vec(-1 + (2.0 / WIDTH) * x , 
                         1 - (2.0 / HEIGHT) * y, 
                         1));
                         */
            /*
            Ray ray(vec(0, 0, 0), 
                    vec(-2 + (4.0 / WIDTH) * x , 
                         1 - (2.0 / HEIGHT) * y, 
                         1));
             */
            /*
            Ray ray(vec(0, 0, -5), 
                    vec(-0.666 + (1.333 / WIDTH) * x , 
                         0.5 - (1.0 / HEIGHT) * y, 
                         1));
                         */
            Ray ray(vec(0, 6, -8), 
                    vec(-0.666 + (1.333 / WIDTH) * x , 
                         0.0 - (1.0 / HEIGHT) * y, 
                         1));

            Primitive *prim = 0;
            double length  = -1;
            for (int i = 0; i < prim_count; i++) {
                double tmp = prims[i]->intercept(ray);
                if ( (tmp > 0) && ((length < 0) || (tmp < length)) ) {
                    prim = prims[i];
                    length = tmp;
                }
            }

            if (prim) {
                // interception point...
                vec p = ray.dir * length;
                p = p + ray.pos;
                vec toLight = light.pos - p;

                // Cast ray from interception point to light source,
                // and check if object is between...
                Ray sray(p, toLight);
                bool hit = false;
                for (int i = 0; i < prim_count; i++) {
                    if (prims[i] == prim)
                        continue;
                    if (prims[i]->intercept(sray) > 0) {
                        hit = true;
                        break;
                    }
                }

                if (!hit) {
                    // viewer vector...
                    vec v = ray.dir * -1;
                    v.normal();
                    // light vector...
                    vec l = light.pos - p;
                    double len = l.mag();
                    l.normal();
                    // normal vector...
                    vec n = prim->normalAt(p);
                    n.normal();
                    // halfway vector...
                    vec tmp =  v + l;
                    vec h   = tmp / tmp.mag();

                    tmp = prim->colorAt(p);
                    tmp = tmp * vec(0.2, 0.2, 0.2);

                    double i = max(1.0 - (len / 30.0), 0.0);
                    tmp = tmp * i;

                    vec col = tmp * ldexp(max(n.dot(h), 0.0), 3);

                    data[x][y] = col;
                } else {
                    vec tmp = prim->colorAt(p);
                    tmp = tmp * vec(0.1, 0.1, 0.1);
                    data[x][y] = tmp;
                }
            } else {
                data[x][y] = vec(0, 0, 0);
            }
        }
        repaint(0, y, WIDTH, 1);
    }

    std::cout << "Finished!" << std::endl;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rect = event->rect();
    int y1 = rect.y(),
        y2 = y1 + rect.height(),
        x1 = rect.x(),
        x2 = x1 + rect.width();
    for (int y = y1; y < y2; y++) {
        for (int x = x1; x < x2; x++) {
            painter.setPen(QColor(min(255, max(0, (int)(data[x][y].x * 255))), 
                                  min(255, max(0, (int)(data[x][y].y * 255))), 
                                  min(255, max(0, (int)(data[x][y].z * 255)))));
            painter.drawPoint(x, y);
        }
    }
}
