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
    void dda(const QPoint *, const QPoint *, QPainter *);
    void bresenham(const QPoint *, const QPoint *, QPainter *);

protected:

};

#endif // LINE_H
