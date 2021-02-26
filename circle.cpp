#include "circle.h"
#include <math.h>

Circle::Circle()
{

}

/**
 * @brief Circle::bresenham
 * Function to implement the Bresenham algorithm of a Circle.
 */
void Circle::bresenham(QPoint *firstPoint, QPoint *secondPoint, QImage *canvas){
    int xc, yc, r,
        x,y,d;

    //Inicialization.

    r = sqrt(((secondPoint->x()-firstPoint->x())*(secondPoint->x()-firstPoint->x()))
             -
             ((secondPoint->y()-firstPoint->y())*(secondPoint->y()-firstPoint->y())));
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
void Circle::draw(int xc, int yc, int x, int y, QImage *canvas)
{
    canvas->setPixel(xc+x, yc+y, qRgb(255,0,0));
    canvas->setPixel(xc-x, yc+y, qRgb(255,0,0));
    canvas->setPixel(xc+x, yc-y, qRgb(255,0,0));
    canvas->setPixel(xc-x, yc-y, qRgb(255,0,0));
    canvas->setPixel(xc+y, yc+x, qRgb(255,0,0));
    canvas->setPixel(xc-y, yc+x, qRgb(255,0,0));
    canvas->setPixel(xc+y, yc-x, qRgb(255,0,0));
    canvas->setPixel(xc-y, yc-x, qRgb(255,0,0));
}
