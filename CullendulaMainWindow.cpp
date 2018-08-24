//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

// own includes
#include "CullendulaMainWindow.h"
#include "ui_CullendulaMainWindow.h"

// Qt includes
#include <QtGui/QDropEvent>
#include <QtGui/QPixmap>
#include <QtWidgets/QMessageBox>
#include <QtCore/QMimeData>
#include <QtCore/QDebug> //todom maybe remove

//----------------------------------------------------------------------------

CullendulaMainWindow::CullendulaMainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CullendulaMainWindow)
{
    setAcceptDrops(true);
    ui->setupUi(this);

    // make the current version information visible to the user - as long as no menu with help&stuff exists
    setWindowTitle(windowTitle().append(c_versionString));

    // disable the buttons immediately; until the directory is set
    activateButtons(false);

    // connects for the useable push-buttons (and their hotkeys)
    connect(ui->leftPB, &QPushButton::clicked, this, &CullendulaMainWindow::slotButtonLeftTriggered);
    connect(ui->rightPB, &QPushButton::clicked, this, &CullendulaMainWindow::slotButtonRightTriggered);
    connect(ui->savePB, &QPushButton::clicked, this, &CullendulaMainWindow::slotButtonSaveTriggered);
    connect(ui->trashPB, &QPushButton::clicked, this, &CullendulaMainWindow::slotButtonTrashTriggered);

    // necessary! even if the QLabel itself accepts already drops
    setAcceptDrops(true);

    // create the menu
    createActions();
    createMenus();

    printStatus("system is up and running :)");
}

//----------------------------------------------------------------------------

CullendulaMainWindow::~CullendulaMainWindow()
{
    delete ui;
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();

    printStatus("drop current load and let's see what you dragged?");
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();

    // check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls())
    {
        const QList<QUrl> urlList = mimeData->urls();

        // extract the local paths of the files: print all of them; can be removed laters
        qDebug() << "new drop:";
        for (auto const& url : urlList)
        {
            qDebug() << "\tdropped: " << url;
        }

        // just use the very first one ..
        if(!urlList.isEmpty())
        {
            bool const success = m_fileSystemHandler.setWorkingPath(urlList.first().path());
            if(success)
            {
                refreshLabel();
            }
        }
    }
    else
    {
        printStatus("The load was not usable! :(");
    }

    event->acceptProposedAction();
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::resizeEvent(QResizeEvent* event)
{
    qDebug() << "CullendulaMainWindow::resizeEvent()";

    QMainWindow::resizeEvent(event);

    // reload the current picture (or text)
    refreshLabel();
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::slotButtonLeftTriggered()
{
    qDebug() << "CullendulaMainWindow::slotButtonLeftTriggered()";

    bool const success = m_fileSystemHandler.switchCurrentPositionToTheLeft();
    // check if that was successful and then refresh
    if(success)
    {
        refreshLabel();
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::slotButtonRightTriggered()
{
    qDebug() << "CullendulaMainWindow::slotButtonRightTriggered()";

    bool const success = m_fileSystemHandler.switchCurrentPositionToTheRight();
    // check if that was successful and then refresh
    if(success)
    {
        refreshLabel();
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::slotButtonSaveTriggered()
{
    qDebug() << "CullendulaMainWindow::slotButtonSaveTriggered()";

    // move the current file to the output-folder
    bool const success = m_fileSystemHandler.saveCurrentFile();
    if(success)
    {
        refreshLabel();
    }
    else
    {
        qDebug() << "\tERROR: moving file did not succeed";
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::slotButtonTrashTriggered()
{
    qDebug() << "CullendulaMainWindow::slotButtonTrashTriggered()";

    // move the current file to the output-folder
    bool const success = m_fileSystemHandler.trashCurrentFile();
    if(success)
    {
        refreshLabel();
    }
    else
    {
        qDebug() << "\tERROR: moving file did not succeed";
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::loadAndScalePhoto(QString const& path)
{
    qDebug() << "CullendulaMainWindow::loadAndScalePhoto(): path=" << path;

    QPixmap const pixmap = QPixmap(path);

    // scale while keeping the aspect ratio
    int width = ui->centerLabel->width() - c_extraWidthBecauseOfFraming;
    int height = ui->centerLabel->height() - c_extraWidthBecauseOfFraming;
    qDebug() << "label-size:" << width << "*" << height;

    // prevent upscaling of smaller photos
    if(pixmap.width() < width)
    {
        width = pixmap.width();
    }

    if(pixmap.height() < height)
    {
        height = pixmap.height();
    }

    // assert that both values are positive
    width = std::max(0, width);
    height = std::max(0, height);

    // set a scaled pixmap keeping its aspect ratio
    ui->centerLabel->setPixmap(pixmap.scaled(width, height, Qt::KeepAspectRatio));
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::refreshLabel()
{
    qDebug() << "CullendulaMainWindow::refreshLabel():";

    QString const path = m_fileSystemHandler.getCurrentImagePath();
    if(path.isEmpty()) // just the case if no valid images found
    {
        ui->centerLabel->setText("no more valid images found: work maybe finished? :)\ndrag&drop the next folder or files if you want!");

        activateButtons(false);

        printStatus("no more files");
    }
    else
    {
        if(QFile::exists(path))
        {
            // load the file
            loadAndScalePhoto(path);

            activateButtons(true);

            // print the current file-path as user-notification
            QString const message = m_fileSystemHandler.getCurrentStatus() + ": " + path;
            printStatus(message);
        }
        else
        {
            qDebug() << "CullendulaMainWindow::refreshLabel(): given path did not exist: " << path;
        }
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::activateButtons(const bool active)
{
    // en-/disable all four buttons
    ui->leftPB->setEnabled(active);
    ui->rightPB->setEnabled(active);
    ui->savePB->setEnabled(active);
    ui->trashPB->setEnabled(active);
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::printStatus(const QString & message)
{
    // messages shall disappear after five seconds
    ui->statusBar->showMessage(message, c_StatusBarDelay);
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::createActions()
{
    aboutAct = new QAction(tr("&About Cullendula"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &CullendulaMainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
    //connect(aboutQtAct, &QAction::triggered, this, &CullendulaMainWindow::aboutQt);
    connect(aboutQtAct, &QAction::triggered, this, [=] () { printStatus(tr("Invoked <b>Help|About Qt</b>")); }); // replaced the slot-call with a lambda :)
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::createMenus()
{
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::about()
{
    printStatus(tr("Invoked Help|About"));
    QMessageBox mBox(QMessageBox::Information, tr("About Cullendula"),
                     tr("Helper program to sort out (\"cull\") a collection of pictures in a directory after a nice photo-walk or event.<br>"
                        "Should work cross-platform.<br>"
                        "<br>"
                        "Developed by <a href='mail@marcelpetrick.it'>mail@marcelpetrick.it</a><br>"
                        "Sourcecode can be found inside the repository at <a href='https://github.com/marcelpetrick/Cullendula/'>https://github.com/marcelpetrick/Cullendula</a><br>"
                        "Feel free to use and share: GPL v3 :3"));
    mBox.setTextFormat(Qt::RichText);
    mBox.exec();
}

//----------------------------------------------------------------------------

//void CullendulaMainWindow::aboutQt()
//{
//    printStatus(tr("Invoked <b>Help|About Qt</b>"));
//}

//----------------------------------------------------------------------------
