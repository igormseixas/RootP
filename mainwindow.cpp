#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainterPath>

MainWindow::MainWindow()
{
    line = new Line();
    circle = new Circle();

    setWindowTitle(tr("RootP"));
    resize(800, 600);

    m_tile = QPixmap(128, 128);
    m_tile.fill(Qt::white);
    QPainter pt(&m_tile);
    QColor color(230, 230, 230);
    pt.fillRect(0, 0, 64, 64, color);
    pt.fillRect(64, 64, 64, 64, color);
    pt.end();

    createActions();
    createMenus();
    QImage canvas(size(), QImage::Format_ARGB32_Premultiplied);
    updateCanvas = canvas;
    updateCanvas.fill(bg_image_color);
    //updateCanvas.fill(Qt::transparent);

}

void MainWindow::paintEvent(QPaintEvent *event){

    static QImage *static_image = nullptr;

    QPainter painter;

    if (preferImage()){
        if (!static_image || static_image->size() != size()) {
            delete static_image;
            static_image = new QImage(size(), QImage::Format_RGB32);
            static_image->fill(qRgb(255,255,255));
        }
        painter.begin(static_image);

        int o = 10;

        QBrush bg = palette().brush(QPalette::Window);
        painter.fillRect(0, 0, o, o, bg);
        painter.fillRect(width() - o, 0, o, o, bg);
        painter.fillRect(0, height() - o, o, o, bg);
        painter.fillRect(width() - o, height() - o, o, o, bg);
    }else{
        static_image = new QImage(size(), QImage::Format_RGB32);
        static_image->fill(qRgb(255,255,255));
        painter.begin(this);
    }

    painter.setClipRect(event->rect());

    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath clipPath;

    QRect r = rect();
    qreal left = r.x() + 1;
    qreal top = r.y() + 23;
    qreal right = r.right();
    qreal bottom = r.bottom();
    qreal radius2 = 8 * 2;

    clipPath.moveTo(right - radius2, top);
    clipPath.arcTo(right - radius2, top, radius2, radius2, 90, -90);
    clipPath.arcTo(right - radius2, bottom - radius2, radius2, radius2, 0, -90);
    clipPath.arcTo(left, bottom - radius2, radius2, radius2, 270, -90);
    clipPath.arcTo(left, top, radius2, radius2, 180, -90);
    clipPath.closeSubpath();

    painter.save();
    painter.setClipPath(clipPath, Qt::IntersectClip);

    painter.drawTiledPixmap(rect(), m_tile);

    // client painting

    paint(&painter);

    painter.restore();

    painter.save();

    int level = 180;
    painter.setPen(QPen(QColor(level, level, level), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(clipPath);

    if(preferImage()){
        painter.end();
        painter.begin(this);
        painter.drawImage(event->rect(), *static_image, event->rect());
    }

    painter.drawImage(QPoint(0,0), updateCanvas);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    if (width() > updateCanvas.width() || height() > updateCanvas.height()) {
        int newWidth = qMax(width() + 128, updateCanvas.width());
        int newHeight = qMax(height() + 128, updateCanvas.height());
        resizeImage(&updateCanvas, QSize(newWidth, newHeight));
        update();
    }
    QMainWindow::resizeEvent(event);
}

void MainWindow::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_ARGB32_Premultiplied);
    newImage.fill(bg_image_color);
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (counter == 0 ) {
        firstPoint = event->pos();
        counter ++;
    }else{
        secondPoint = event->pos();
        counter = 0;
    }
}

void MainWindow::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);

    transformationMenu = menuBar()->addMenu(tr("&Transformation"));

    rasterizationMenu = menuBar()->addMenu(tr("&Rasterization"));
    rasterizationMenu->addAction(lineddaAct);
    rasterizationMenu->addAction(linebresenhamAct);
    rasterizationMenu->addAction(circlebresenhamAct);

    cutsMenu = menuBar()->addMenu(tr("Cuts"));

    helpMenu = menuBar()->addMenu(tr("&Help"));
    //helpMenu->addAction(aboutAct);
}

void MainWindow::createActions(){
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip("Create a new file");
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    lineddaAct = new QAction(tr("&Line DDA"), this);
    lineddaAct->setStatusTip("Draw a line using a DDA Algorithm");
    connect(lineddaAct, &QAction::triggered, this, &MainWindow::lineDDA);

    linebresenhamAct = new QAction(tr("&Line Bresenham"), this);
    linebresenhamAct->setStatusTip("Draw a line using a Bresenham Algorithm");
    connect(linebresenhamAct, &QAction::triggered, this, &MainWindow::lineBresenham);

    circlebresenhamAct = new QAction(tr("&Circle Bresenham"), this);
    circlebresenhamAct->setStatusTip("Draw a circle using a Bresenham Algorithm");
    connect(circlebresenhamAct, &QAction::triggered, this, &MainWindow::circleBresenham);
}

void MainWindow::newFile()
{
    infoLabel->setText(tr("Invoked <b>File|New</b>"));
}

void MainWindow::lineDDA(){
    line->dda(&firstPoint, &secondPoint, &updateCanvas);
    update();
}

void MainWindow::lineBresenham(){
    line->bresenham(&firstPoint, &secondPoint, &updateCanvas);
    update();
}

void MainWindow::circleBresenham(){
    circle->bresenham(&firstPoint, &secondPoint, &updateCanvas);
    update();
}

