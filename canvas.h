#ifndef CANVAS_H
#define CANVAS_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QPainterPath>

#include <line.h>
#include <circle.h>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    Canvas(QWidget *parent = 0);

    // Handles all events
    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    // Has the image been modified since last save
    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }

public slots:

    // Events to handle
    void clearImage();
    void print();

    // Menus selections.
    void setFreeDraw(bool new_mFreeDraw);
    void setSelect(bool new_mSelect);
    void setTransformationTranslate(bool new_mTransformationTranslate);
    void setTransformationRotate(bool new_mTransformationRotate);
    void setTransformationScale(bool new_mTransformationScale);

    void setLineDDA(bool new_mLinedda);
    void setLineBresenham(bool new_mLineBresenham);
    void setCircleBresenham(bool new_mCircleBresenham);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    // Updates the scribble area where we are painting
    void paintEvent(QPaintEvent *event) override;

    // Makes sure the area we are drawing on remains
    // as large as the widget
    void resizeEvent(QResizeEvent *event) override;

private:
    QPainterPath pp;

    //Mouse events.
    QPoint firstPoint, secondPoint;
    int counter = 0;
    //Selection Rectangle.
    bool selectionStarted;
    QRect selectionRect;

    void resizeImage(QImage *image, const QSize &newSize);

    void drawLineTo(const QPoint &endPoint);

    void lineDDA(const QPoint &fPoint, const QPoint &sPoint);
    void lineBresenham(const QPoint &fPoint, const QPoint &sPoint);
    void circleBresenham(const QPoint &fPoint, const QPoint &sPoint);

    Line *line;
    Circle *circle;

    // Define the color and oppacity of background color.
    const int oppacity = 0;
    const QColor bg_image_color = QColor(0,0,0,oppacity);

    // Will be marked true or false depending on if
    // we have saved after a change
    bool modified;

    // Marked true or false depending on if the user
    // is drawing
    bool scribbling;

    // Define in wich menu are.
    bool m_freedraw;
    bool m_select;
    bool m_transformationTranslate;
    bool m_transformationRotate;
    qreal rotation;
    bool m_transformationScale;
    qreal scalation;
    bool m_linedda;
    bool m_linebresenham;
    bool m_circlebresenham;

    // Holds the current pen width & color
    int myPenWidth;
    QColor myPenColor;

    // Stores the image being drawn
    QImage image;

    // Stores the location at the current mouse event
    QPoint lastPoint;
};

#endif // CANVAS_H
