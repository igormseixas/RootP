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
    virtual void paint(QPainter *) {}

    QImage updateCanvas;

    bool preferImage() const { return m_prefer_image; }

public slots:

    void setPreferImage(bool pi) { m_prefer_image = pi; }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;

    QPixmap m_tile;
    bool m_prefer_image;

private slots:
    void newFile();
    void lineDDA();
    void lineBresenham();
    void circleBresenham();

private:
    void createMenus();
    void createActions();
    void resizeImage(QImage *image, const QSize &newSize);

    const int oppacity = 0;
    const QColor bg_image_color = QColor(0,0,0,oppacity);

    Line *line;
    Circle *circle;

    QPoint firstPoint, secondPoint;
    int counter = 0;

    QMenu *fileMenu;
    QMenu *transformationMenu;
    QMenu *reflectionMenu;
    QMenu *rasterizationMenu;
    QMenu *cutsMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *saveAct;
    QAction *saveasAct;
    QAction *exitAct;

    QAction *translateAct;
    QAction *rotateAct;
    QAction *scaleAct;
    QAction *reflectxAct;
    QAction *reflectyAct;
    QAction *reflectxyAct;

    QAction *lineddaAct;
    QAction *linebresenhamAct;
    QAction *circlebresenhamAct;

    QAction *cohensutherAct;
    QAction *liangbarskyAct;

    QAction *aboutAct;

    QLabel *infoLabel;
};
#endif // MAINWINDOW_H
