#ifndef LINE_H
#define LINE_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QMouseEvent>

class Line : public QWidget
{
    Q_OBJECT
public:
    Line();
    QRgb renk;

    //Algorithms
    void dda(QPoint *, QPoint *, QImage *);
    void bresenham(QPoint *, QPoint *, QImage *);

protected:

};

#endif // LINE_H
