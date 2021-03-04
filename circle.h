#ifndef CIRCLE_H
#define CIRCLE_H

#include <QMouseEvent>
#include <QImage>

class Circle
{
public:
    Circle();

    //Algorithms
    void bresenham(const QPoint *, const QPoint *, QPainter *);

private:
    void draw(int, int, int, int, QPainter *);
};

#endif // CIRCLE_H
