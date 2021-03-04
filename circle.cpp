#include <QPainter>
#include <QDebug>

#include "circle.h"
#include <math.h>

Circle::Circle()
{

}

/**
 * @brief Circle::bresenham
 * Function to implement the Bresenham algorithm of a Circle.
 */
void Circle::bresenham(const QPoint *firstPoint, const QPoint *secondPoint, QPainter *canvas){
    int xc, yc, r,
        x,y,d;

    //Inicialization.
    r = sqrt(abs(((secondPoint->x()-firstPoint->x())*(secondPoint->x()-firstPoint->x()))
                 +
                 ((secondPoint->y()-firstPoint->y())*(secondPoint->y()-firstPoint->y()))));

    x = 0; y = r;
    d = 3-2*r;
    xc = firstPoint->x();
    yc = firstPoint->y();

    //Draw the pixel and symetrical pixels.
    Circle::draw(xc, yc, x, y, canvas);

    //Draw the arc and complete with symetrical pixels.
    while(y >= x){
        x++;
        if(d > 0){
           y--;
           d = d + 4*(x-y) + 10;
        }else{
            d = d + 4*x + 6;
        }
        Circle::draw(xc, yc, x, y, canvas);
    }
}

/**
 * @brief Circle::draw
 * Function to draw other 7 symetric pixel.
 * @param xc
 * @param yc
 * @param x
 * @param y
 */
void Circle::draw(int xc, int yc, int x, int y, QPainter *canvas)
{
    canvas->drawPoint(xc+x, yc+y);
    canvas->drawPoint(xc-x, yc+y);
    canvas->drawPoint(xc+x, yc-y);
    canvas->drawPoint(xc-x, yc-y);
    canvas->drawPoint(xc+y, yc+x);
    canvas->drawPoint(xc-y, yc+x);
    canvas->drawPoint(xc+y, yc-x);
    canvas->drawPoint(xc-y, yc-x);
}
