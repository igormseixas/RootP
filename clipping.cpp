#include "clipping.h"

Clipping::Clipping()
{
    //Empty constructor.
    xmin = 0;
    ymin = 0;
    xmax = 0;
    ymax = 0;
}

Clipping::Clipping( QPoint *topLeftMin, QPoint *bottonRightMax )
{
    //Construct the diagonal of clipping área.
    xmin = topLeftMin->x();
    ymin = topLeftMin->y();

    xmax = bottonRightMax->x();
    ymax = bottonRightMax->y();
}

// Function to compute region code for a point(x, y)
int Clipping::region_code(qreal x, qreal y)
{
    // Initialized as being inside.
    int code = INSIDE; // 0000

    if (x < xmin) // to the left of rectangle
        code |= LEFT; // 0001
    else if (x > xmax) // to the right of rectangle
        code |= RIGHT; //0010
    if (y < ymin) // below the rectangle
        code |= BOTTOM; //0100
    else if (y > ymax) // above the rectangle
        code |= TOP; //1000

    return code;
}

/**
 * @brief Clipping::cohenSutherland
 * Function to implement Cohen-Sutherland Algorithm.
 * @param firstPoint
 * @param secondPoint
 * @param canvas
 */
void Clipping::cohenSutherland(const QPoint *firstPoint, const QPoint *secondPoint, QPainter *canvas){
    //Variables.
    int x1, y1, x2, y2,
        code1, code2;
    bool accept = false,
         done = false;
    Line lineCohenSutherland;

    //Inicialization.
    x1 = firstPoint->x();  y1 = firstPoint->y();
    x2 = secondPoint->x(); y2 = secondPoint->y();

    //Compute regions.
    code1 = region_code(x1,y1);
    code2 = region_code(x2,y2);

    while(done==false){
        if((code1==0) && (code2==0)){
            //Both endpoints lie within rectangle
            accept = true;
            done = true;
        }else if(code1 & code2){
            done = true;
        }else{
            //Loop Variables.
            int code_out;
            qreal x, y;
            //At least one endpoint is outside the rectangle.
            if(code1!=0){
                code_out=code1;
            }else{
                code_out=code2;
            }

            //Finding intersections points.
            if(code_out & TOP){
                //Point is above the clip rectangle.
                x = x1 + (x2-x1) * (ymax-y1) / (y2-y1);
                y = ymax;
            }else if(code_out & BOTTOM){
                //Point is below the rectangle.
                x = x1 + (x2-x1) * (ymin-y1) / (y2-y1);
                y = ymin;
            }else if(code_out & RIGHT){
                //Point is to the right of the rectangle.
                y = y1 + (y2-y1) * (xmax-x1) / (x2-x1);
                x = xmax;
            }else if(code_out & LEFT){
                //Point is to the left of the rectangle.
                y = y1 + (y2-y1) * (xmin-x1) / (x2-x1);
                x = xmin;
            }

            // Now intersection point x, y is found.
            // Replace point outside rectangle by intersection point.
            if(code_out==code1){
                x1 = x;
                y1 = y;
                code1 = region_code(x1,y1);
            }else{
                x2 = x;
                y2 = y;
                code2 = region_code(x2,y2);
            }
        }
    }
    //Draw the new line.
    if(accept){
        const QPoint point1(x1,y1), point2(x2,y2);
        lineCohenSutherland.bresenham(&point1, &point2, canvas);
    }
}
