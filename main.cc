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
