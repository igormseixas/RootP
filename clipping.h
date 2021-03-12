#ifndef CLIPPING_H
#define CLIPPING_H

#include <QImage>
#include <QPoint>

#include <line.h>

class Clipping
{
public:
    Clipping();
    Clipping( QPoint*, QPoint *);

    //Algorithms
    void cohenSutherland(const QPoint *, const QPoint *, QPainter *);

private:
    // Global Vars
    int xmin;
    int ymin;
    int xmax;
    int ymax;

    // Defining region codes
    const int INSIDE = 0; // 0000
    const int LEFT = 1; // 0001
    const int RIGHT = 2; // 0010
    const int BOTTOM = 4; // 0100
    const int TOP = 8; // 1000

    int region_code(qreal x, qreal y);
};

#endif // CLIPPING_H
