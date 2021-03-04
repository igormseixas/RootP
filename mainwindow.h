#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>

#include <canvas.h>

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

public slots:

protected:
    //void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    bool preferWhiteBackground() const { return m_prefer_image; }

    QPixmap m_tile;
    bool m_prefer_image = false; //True if prefer White BackGround

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void about();

    void transformationTranslate();

private:
    Canvas *canvas;

    void createMenus();
    void createActions();

    const int oppacity = 0;
    const QColor bg_image_color = QColor(0,0,0,oppacity);

    // Will check if changes have occurred since last save
    bool maybeSave();
    // Opens the Save dialog and saves
    bool saveFile(const QByteArray &fileFormat);

    QPoint firstPoint, secondPoint;
    int counter = 0;

    bool selectionStarted;
    QRect selectionRect;

    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *transformationMenu;
    QMenu *reflectionMenu;
    QMenu *rasterizationMenu;
    QMenu *cutsMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *openAct;
    QList<QAction *> saveAsActs;
    QAction *printAct;
    QAction *exitAct;

    QAction *penColorAct;
    QAction *penWidthAct;

    QAction *translateAct;
    QAction *rotateAct;
    QAction *scaleAct;
    QAction *reflectxAct;
    QAction *reflectyAct;
    QAction *reflectxyAct;

    QActionGroup *rasterizationMenuGroup;
    QAction *lineddaAct;
    QAction *linebresenhamAct;
    QAction *circlebresenhamAct;

    QAction *cohensutherAct;
    QAction *liangbarskyAct;

    QAction *aboutAct;

    QLabel *infoLabel;
};
#endif // MAINWINDOW_H
