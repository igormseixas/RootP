#ifndef CIRCLE_H
#define CIRCLE_H

#include <QMouseEvent>
#include <QImage>

class Circle
{
public:
    Circle();

    //Algorithms
    void bresenham(QPoint *, QPoint *, QImage *);

private:
    void draw(int, int, int, int, QImage *);
};

#endif // CIRCLE_H
