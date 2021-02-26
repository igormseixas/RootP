#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow()
{
    line = new Line();
    circle = new Circle();

    QImage canvas(800,600,QImage::Format_RGB32);
    updateCanvas = canvas;

    setWindowTitle(tr("RootP"));
    resize(800, 600);


    createActions();
    createMenus();
}

void MainWindow::paintEvent(QPaintEvent *){
    painter.begin(this);
    painter.drawPixmap(0,0,QPixmap::fromImage(updateCanvas));
    painter.end();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
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

