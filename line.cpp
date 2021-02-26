#include "line.h"
#include <math.h>

Line::Line()
{
    //renk = qRgb(255,0,0);
}

/**
 * @brief Line::dda
 * Function to implement the DDA algorithm of a Line.
 */
void Line::dda(QPoint *firstPoint, QPoint *secondPoint, QImage *canvas){
    //Variables.
    int dx, dy, k;
    double x_incr, y_incr, x, y, steps;

    //Inicialization.
    dx = secondPoint->x() - firstPoint->x();
    dy = secondPoint->y() - firstPoint->y();

    //Drawing.
    if(abs(dx) > abs(dy)){
        steps = abs(dx);
    } else{
        steps = abs(dy);
    }

    //Increment.
    x_incr = dx/steps;
    y_incr = dy/steps;
    x = firstPoint->x(); y = firstPoint->y();

    //Set Pixel.
    canvas->setPixel(round(x),round(y), qRgb(0,255,0));
    //update();
    //QApplication::processEvents();

    for(k=0; k<steps; ++k){
        x = x+x_incr;
        y = y+y_incr;

        canvas->setPixel(round(x),round(y), qRgb(0,255,0));
        //update();
    }
}

/**
 * @brief Line::bresenham
 * Function to implement the Bresenham algorithm of a Line.
 */
void Line::bresenham(QPoint *firstPoint, QPoint *secondPoint, QImage *canvas){
    //Variables.
    int x1, y1, x2, y2,
        dx, dy, x, y, i,
        const1, const2, p,
        incrx, incry;

    //Inicialization.
    x1 = firstPoint->x();  y1 = firstPoint->y();
    x2 = secondPoint->x(); y2 = secondPoint->y();
    dx = x2-x1;
    dy = y2-y1;


    if (dx >= 0)
        incrx = 1;
    else{
        incrx= -1;
        dx= -dx;
    }
    if (dy >= 0)
        incry = 1;
    else{
        incry= -1;
        dy= -dy;
    }
    x = x1; y = y1;

    //Set Pixel.
    canvas->setPixel(round(x),round(y), qRgb(255,0,0));

    if (dy < dx){
        p = 2*dy-dx;
        const1 = 2*dy;
        const2 = 2*(dy-dx);

        for (i=0; i < dx; i++) {
            x += incrx;
        if (p < 0)
            p += const1;
        else {
            y += incry;
            p+= const2;
        }

        //Set Pixel.
        canvas->setPixel(round(x),round(y), qRgb(255,0,0));
        }
    }else {
        p = 2*dx-dy;
        const1 = 2*dx;
        const2 = 2*(dx-dy);
        for (i=0; i < dy; i++) {
            y += incry;
            if (p < 0)
                p += const1;
            else {
                x += incrx;
            p+= const2;
        }
        //Set Pixel.
        canvas->setPixel(round(x),round(y), qRgb(255,0,0));
        }
    }
}
