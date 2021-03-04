#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    // Roots the widget to the top left even if resized
    setAttribute(Qt::WA_StaticContents);

    // Set defaults for the monitored variables
    modified = false;
    scribbling = false;
    myPenWidth = 3;
    myPenColor = Qt::black;

    //Set the access menus as false
    m_freedraw = false;
    m_linedda = false;
    m_linebresenham = false;
    m_circlebresenham = false;
}

// Used to load the image and place it in the widget
bool Canvas::openImage(const QString &fileName)
{
    // Holds the image
    QImage loadedImage;

    // If the image wasn't loaded leave this function
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

// Save the current image
bool Canvas::saveImage(const QString &fileName, const char *fileFormat)
{
    // Created to hold the image
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}

// Used to change the pen color
void Canvas::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

// Used to change the pen width
void Canvas::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

// Used to set if the menu Line DDA is accessed.
void Canvas::setLineDDA(bool new_mLinedda)
{
    m_linedda = new_mLinedda;
    update();
}

// Used to set if the menu Line Bresenham is accessed.
void Canvas::setLineBresenham(bool new_mLineBresenham)
{
    m_linebresenham = new_mLineBresenham;
}

// Used to set if the menu Circle Bresenham is accessed.
void Canvas::setCircleBresenham(bool new_mCircleBresenham)
{
    m_circlebresenham = new_mCircleBresenham;
}

// Color the image area with white
void Canvas::clearImage()
{
    image.fill(bg_image_color);
    modified = true;
    update();
}

// If a mouse button is pressed check if it was the
// left button and if so store the current position
// Set that we are currently drawing
void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (counter == 0 ) {
        firstPoint = event->pos();
        counter ++;
    }else{
        secondPoint = event->pos();
        counter = 0;
    }

    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
    }
}

// When the mouse moves if the left button is clicked
// we call the drawline function which draws a line
// from the last position to the current
void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling && m_freedraw)
        drawLineTo(event->pos());
}

// If the button is released we set variables to stop drawing
void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    // Free Draw
    if (event->button() == Qt::LeftButton && scribbling && m_freedraw) {
        drawLineTo(event->pos());
        scribbling = false;
    }

    // Line DDA
    if (event->button() == Qt::LeftButton && scribbling && m_linedda && counter == 0) {
        lineDDA(firstPoint, secondPoint);
        scribbling = false;
    }

    // Line Bresenham
    if (event->button() == Qt::LeftButton && scribbling && m_linebresenham && counter == 0) {
        lineBresenham(firstPoint, secondPoint);
        scribbling = false;
    }

    // Circle Bresenham
    if (event->button() == Qt::LeftButton && scribbling && m_circlebresenham && counter == 0) {
        circleBresenham(firstPoint, secondPoint);
        scribbling = false;
    }
}

// QPainter provides functions to draw on the widget
// The QPaintEvent is sent to widgets that need to
// update themselves
void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Returns the rectangle that needs to be updated
    QRect dirtyRect = event->rect();

    // Draws the rectangle where the image needs to
    // be updated
    painter.drawImage(dirtyRect, image, dirtyRect);
}

// Resize the image to slightly larger then the main window
// to cut down on the need to resize the image
void Canvas::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void Canvas::drawLineTo(const QPoint &endPoint)
{
    // Used to draw on the widget
    QPainter painter(&image);

    // Set the current settings for the pen
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    // Draw a line from the last registered point to the current
    painter.drawLine(lastPoint, endPoint);

    // Set that the image hasn't been saved
    modified = true;

    int rad = (myPenWidth / 2) + 2;

    // Call to update the rectangular space where we drew
    update(QRect(lastPoint, endPoint).normalized()
           .adjusted(-rad, -rad, +rad, +rad));

    // Update the last position where we left off drawing
    lastPoint = endPoint;
}

// When the app is resized create a new image using
// the changes made to the image
void Canvas::resizeImage(QImage *image, const QSize &newSize)
{
    // Check if we need to redraw the image
    if (image->size() == newSize)
        return;

    // Create a new image to display and fill it with white
    QImage newImage(newSize, QImage::Format_ARGB32_Premultiplied);
    newImage.fill(bg_image_color);

    // Draw the image
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

// Print the image
void Canvas::print()
{
    // Check for print dialog availability
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    // Can be used to print
    QPrinter printer(QPrinter::HighResolution);

    // Open printer dialog and print if asked
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_CONFIG(printdialog)
}

//Call for Line DDA Algorithm
void Canvas::lineDDA(const QPoint &fPoint, const QPoint &sPoint)
{
    // Used to draw on the widget
    QPainter painter(&image);

    // Set the current settings for the pen
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    // Used to draw on the widget
    line = new Line();
    line->dda(&fPoint, &sPoint, &painter);
    update();

    // Set that the image hasn't been saved
    modified = true;
}

//Call for Line Bresenham Algorithm
void Canvas::lineBresenham(const QPoint &fPoint, const QPoint &sPoint)
{
    // Used to draw on the widget
    QPainter painter(&image);

    // Set the current settings for the pen
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    // Used to draw on the widget
    line = new Line();
    line->bresenham(&fPoint, &sPoint, &painter);
    update();

    // Set that the image hasn't been saved
    modified = true;
}

//Call for Circle Bresenham Algorithm
void Canvas::circleBresenham(const QPoint &fPoint, const QPoint &sPoint)
{
    // Used to draw on the widget
    QPainter painter(&image);

    // Set the current settings for the pen
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    // Used to draw on the widget
    circle = new Circle();
    circle->bresenham(&fPoint, &sPoint, &painter);
    update();

    // Set that the image hasn't been saved
    modified = true;
}
