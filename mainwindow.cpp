#include <QtWidgets>
#include <QPainterPath>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow()
{
    canvas = new Canvas;
    setCentralWidget(canvas);

    setWindowTitle(tr("RootP"));
    setMinimumSize(160, 160);
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
    QImage image(size(), QImage::Format_ARGB32_Premultiplied);
    updateCanvas = image;
    updateCanvas.fill(bg_image_color);
    //updateCanvas.fill(Qt::transparent);

}

/*
// User tried to close the app
void MainWindow::closeEvent(QCloseEvent *event)
{
    // If they try to close maybeSave() returns true
    // if no changes have been made and the app closes
    if (maybeSave()) {
        event->accept();
    } else {

        // If there have been changes ignore the event
        event->ignore();
    }
}
*/

// Check if the current image has been changed and then
// open a dialog to open a file
void MainWindow::open()
{
    // Check if changes have been made since last save
    // maybeSave() returns true if no changes have been made
    if (maybeSave()) {

        // Get the file to open from a dialog
        // tr sets the window title to Open File
        // QDir opens the current dirctory
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), QDir::currentPath());

        // If we have a file name load the image and place
        // it in the scribbleArea
        if (!fileName.isEmpty())
            canvas->openImage(fileName);
    }
}

// Called when the user clicks Save As in the menu
void MainWindow::save()
{
    // A QAction represents the action of the user clicking
    QAction *action = qobject_cast<QAction *>(sender());

    // Stores the array of bytes of the users data
    QByteArray fileFormat = action->data().toByteArray();

    // Pass it to be saved
    saveFile(fileFormat);
}

// Opens a dialog to change the pen color
void MainWindow::penColor()
{
    // Store the chosen color from the dialog
    QColor newColor = QColorDialog::getColor(canvas->penColor());

    // If a valid color set it
    if (newColor.isValid())
        canvas->setPenColor(newColor);
}

// Opens a dialog that allows the user to change the pen width
void MainWindow::penWidth()
{
    // Stores button value
    bool ok;

    // tr("Scribble") is the title
    // the next tr is the text to display
    // Get the current pen width
    // Define the min, max, step and ok button
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen width:"),
                                        canvas->penWidth(),
                                        1, 50, 1, &ok);
    // Change the pen width
    if (ok)
        canvas->setPenWidth(newWidth);
}

// Open an about dialog
void MainWindow::about()
{
    // Window title and text to display
    QMessageBox::about(this, tr("About RootP"),
            tr("<p>Computer Graphics App-Project</p>"));
}

void MainWindow::paintEvent(QPaintEvent *event){

    static QImage *static_image = nullptr;
    QPainter painter;

    painter.begin(this);

    painter.setClipRect(event->rect());
    painter.setRenderHint(QPainter::Antialiasing);

    //Draw rounded areas in the Canvas.
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

    //Client painting and make border thicker.
    painter.restore();
    painter.save();

    //Round the corner.
    int level = 180;
    painter.setPen(QPen(QColor(level, level, level), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(clipPath);

    if(preferWhiteBackground()){
        static_image = new QImage(size(), QImage::Format_RGB32);
        static_image->fill(qRgb(255,255,255));

        painter.end();
        painter.begin(this);
        painter.drawImage(event->rect(), *static_image, event->rect());
    }

    painter.drawImage(QPoint(0,0), updateCanvas);
}

void MainWindow::createMenus(){
    // Create Save As option and the list of file types
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach (QAction *action, saveAsActs)
    saveAsMenu->addAction(action);

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = menuBar()->addMenu(tr("&Options"));
    optionMenu->addAction(freeDrawAct);
    optionMenu->addAction(selectAct);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);

    transformationMenu = menuBar()->addMenu(tr("&Transformation"));
    transformationMenu->addAction(translateAct);
    transformationMenu->addAction(rotateAct);
    transformationMenu->addAction(scaleAct);

    reflectionMenu = transformationMenu->addMenu(tr("&Reflection"));
    reflectionMenu->addAction(reflectxAct);
    reflectionMenu->addAction(reflectyAct);
    reflectionMenu->addAction(reflectxyAct);

    rasterizationMenu = menuBar()->addMenu(tr("&Rasterization"));
    rasterizationMenu->addAction(lineddaAct);
    rasterizationMenu->addAction(linebresenhamAct);
    rasterizationMenu->addAction(circlebresenhamAct);

    cutsMenu = menuBar()->addMenu(tr("Cuts"));
    cutsMenu->addAction(cohensutherAct);
    cutsMenu->addAction(liangbarskyAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createActions(){
    //File Menu Acts.
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip("Create a new file");
    connect(newAct, &QAction::triggered, canvas, &Canvas::clearImage);

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);

        action->setData(format);

        connect(action, SIGNAL(triggered()), this, SLOT(save()));

        saveAsActs.append(action);
    }

    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, SIGNAL(triggered()), canvas, SLOT(print()));

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    //END File Menu Acts.

    //Option Menu Acts.
    freeDrawAct = new QAction(tr("Free Draw"), this);
    freeDrawAct->setCheckable(true);
    connect(freeDrawAct, &QAction::toggled, canvas, &Canvas::setFreeDraw);

    selectAct = new QAction(tr("Select"), this);
    selectAct->setCheckable(true);
    connect(selectAct, &QAction::toggled, canvas, &Canvas::setSelect);

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));
    //END Option Menu Acts.

    //Transformation Menu Acts.
    translateAct = new QAction(tr("&Translate"), this);
    translateAct->setStatusTip("Translate image");
    translateAct->setCheckable(true);
    connect(translateAct, &QAction::toggled, canvas, &Canvas::setTransformationTranslate);

    rotateAct = new QAction(tr("&Rotate"), this);

    scaleAct = new QAction(tr("&Scale"), this);

    reflectxAct = new QAction(tr("&Reflection X"), this);

    reflectyAct = new QAction(tr("&Reflection Y"), this);

    reflectxyAct = new QAction(tr("&Reflection XY"), this);
    //END Transformation Menu Acts.

    //Rasterization Menu Acts.
    lineddaAct = new QAction(tr("&Line DDA"), this);
    lineddaAct->setStatusTip("Draw a line using a DDA Algorithm");
    lineddaAct->setCheckable(true);
    //connect(lineddaAct, &QAction::triggered, this, &MainWindow::lineDDA);
    connect(lineddaAct, &QAction::toggled, canvas, &Canvas::setLineDDA);

    linebresenhamAct = new QAction(tr("&Line Bresenham"), this);
    linebresenhamAct->setStatusTip("Draw a line using a Bresenham Algorithm");
    linebresenhamAct->setCheckable(true);
    //connect(linebresenhamAct, &QAction::triggered, this, &MainWindow::lineBresenham);
    connect(linebresenhamAct, &QAction::toggled, canvas, &Canvas::setLineBresenham);

    circlebresenhamAct = new QAction(tr("&Circle Bresenham"), this);
    circlebresenhamAct->setStatusTip("Draw a circle using a Bresenham Algorithm");
    circlebresenhamAct->setCheckable(true);
    connect(circlebresenhamAct, &QAction::toggled, canvas, &Canvas::setCircleBresenham);
    //END Rasterization Menu Acts.

    //Cuts Menu Acts.
    cohensutherAct = new QAction(tr("&Code Regions/Cohen-Sutherland"), this);
    liangbarskyAct = new QAction(tr("&Parametric Equations/Liang-Barsky"), this);
    //END Cuts Menu Acts.

    //Help Menu Acts.
    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    //END Help Menu Acts.

    //Define Groups
    singleMenuGroup = new QActionGroup(this);
    //rasterizationMenuGroup->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);
    singleMenuGroup->addAction(freeDrawAct);
    singleMenuGroup->addAction(selectAct);
    singleMenuGroup->addAction(translateAct);
    singleMenuGroup->addAction(lineddaAct);
    singleMenuGroup->addAction(linebresenhamAct);
    singleMenuGroup->addAction(circlebresenhamAct);
}

bool MainWindow::maybeSave()
{
    // Check for changes since last save
    if (canvas->isModified()) {
       QMessageBox::StandardButton ret;

       // Scribble is the title
       // Add text and the buttons
       ret = QMessageBox::warning(this, tr("Scribble"),
                          tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);

       // If save button clicked call for file to be saved
        if (ret == QMessageBox::Save) {
            return saveFile("png");

        // If cancel do nothing
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    // Define path, name and default file type
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    // Get selected file from dialog
    // Add the proper file formats and extensions
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));

    // If no file do nothing
    if (fileName.isEmpty()) {
        return false;
    } else {

        // Call for the file to be saved
        return canvas->saveImage(fileName, fileFormat.constData());
    }
}

