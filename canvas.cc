#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#include "canvas.h"
#include "vector.h"

class Camera
{
public:
    Camera(const vec &pos, const vec &dir, double hlen, double vlen)
        : pos(pos), dir(dir), hlen(hlen), vlen(vlen) {};

    vec pos;
    vec dir;
    double hlen;
    double vlen;
};

class Ray
{
public:
    Ray(const vec &pos, const vec &dir)
        : pos(pos), dir(dir.normal()) {};

    vec pos;
    vec dir;
};

class Light
{
public:
    Light(const vec &pos, const vec &color, const double power)
        : pos(pos), color(color), power(power) {};

    vec pos;
    vec color;
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
        return (point - pos).normal();
    }
};

class Plane : public Primitive
{
public:
    Plane(const vec &pos, const vec &normal, const vec &color)
        : Primitive(color), pos(pos), normal(normal.normal()) {};

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
        vec p = point + pos;
        int a = (int(fabs(p.z < 0 ? p.z - 1 : p.z)) % 2);
        int b = (int(fabs(p.x < 0 ? p.x - 1 : p.x)) % 2);
        return a == b ? vec(0.6, 0.6, 0.6) : vec(1, 1, 1);
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
    //prims[6] = new Plane(vec(6, 0, 0), vec(-1, 0, 0), vec(0, 1, 1));

    Light light(vec(0.0, 8.0, -1.0), vec(0.2, 0.2, 0.2), 30.0);
    Camera camera(vec(0, 6, -8), vec(0, -1, 1), 1.333, 1.0);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            data[x][y] = vec(1, 0, 1);
        }
    }
    repaint();

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            Ray ray(camera.pos, 
                    vec(-0.666 + (1.333 / WIDTH) * x , 
                         0.0 - (1.0 / HEIGHT) * y, 
                         1));

            Primitive *prim = 0;
            double length  = -1;
            for (int i = 0; i < prim_count; i++) {
                double len = prims[i]->intercept(ray);
                if ( (len > 0) && ((length < 0) || (len < length)) ) {
                    prim = prims[i];
                    length = len;
                }
            }

            if (prim) {
                // hit point in world coordinates
                vec p = (ray.dir * length) + ray.pos;;

                // vector from hit point to light
                vec toLight = light.pos - p;

                // Cast ray from hit point to light source,
                // and check if object is between them...
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
                    // normalized vector from hitpoint to viewer...
                    vec v = (ray.dir * -1).normal();

                    // normalized vector from hitpoint to light...
                    vec l = light.pos - p;
                    double len = l.mag(); // length needed for i below
                    l = l.normal();

                    // normal vector for hitpoint...
                    vec n = prim->normalAt(p).normal();

                    // halfway vector between view and light vector...
                    vec h = (v + l).normal();

                    double i = qMax(1.0 - (len / light.power), 0.0);

                    vec col = prim->colorAt(p)
                            * light.color
                            * i
                            * ldexp(qMax(n.dot(h), 0.0), 3);

                    data[x][y] = col;
                } else {
                    data[x][y] = prim->colorAt(p) * vec(0.1, 0.1, 0.1);
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
            painter.setPen(QColor(qMin(255, qMax(0, (int)(data[x][y].x * 255))), 
                                  qMin(255, qMax(0, (int)(data[x][y].y * 255))), 
                                  qMin(255, qMax(0, (int)(data[x][y].z * 255)))));
            painter.drawPoint(x, y);
        }
    }
}
