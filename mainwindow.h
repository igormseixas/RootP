#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>

#include <line.h>
#include <circle.h>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    QImage updateCanvas;
    QPainter painter;

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void newFile();
    void lineDDA();
    void lineBresenham();
    void circleBresenham();

private:
    void createMenus();
    void createActions();

    Line *line;
    Circle *circle;

    QPoint firstPoint, secondPoint;
    int counter = 0;

    QMenu *fileMenu;
    QMenu *transformationMenu;
    QMenu *rasterizationMenu;
    QMenu *cutsMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *lineddaAct;
    QAction *linebresenhamAct;
    QAction *circlebresenhamAct;

    QLabel *infoLabel;
};
#endif // MAINWINDOW_H
