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
        // load the file
        QPixmap const pixmap = QPixmap(path);

        // scale while keeping the aspect ratio
        int const w = ui->centerLabel->width();
        int const h = ui->centerLabel->height();
        qDebug() << "label-size:" << w << "*" << h;
        // TODO prevent upscaling of smaller photos to big label ..
        // TODO implement the resize-event for the widget

        // set a scaled pixmap keeping its aspect ratio
        ui->centerLabel->setPixmap(pixmap.scaled(w, h, Qt::KeepAspectRatio));

        activateButtons(true);

        printStatus(m_fileSystemHandler.getCurrentStatus());
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

void CullendulaMainWindow::printStatus(const QString message)
{
    // messages shall disappear after five seconds
    ui->statusBar->showMessage(message, 5000);
}
