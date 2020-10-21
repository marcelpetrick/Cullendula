//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

// own includes
#include "CullendulaMainWindow.h"
#include "ui_CullendulaMainWindow.h"

// Qt includes
#include <QtCore/QDebug> //todom maybe remove
#include <QtCore/QMimeData>
#include <QtGui/QDropEvent>
#include <QtGui/QPixmap>
#include <QtWidgets/QMessageBox>

//----------------------------------------------------------------------------

// for constants
namespace {
    //! v0.1 was the basic release; working, but ugly
    //! v0.2 improved useability and stability; more features (move to trash!); refactored code-base; improved code-quality
    //! v0.3 added tooltips; fixed the "pumping center-label"-issue; added menus; fixed some resizing-issues with the image-label
    //! v0.4 added undo/redo-functionality with unit-test; added a nice violet icon for the executable and program
    QString const c_versionString = " - v0.4.4";

    //! Determines how long the status message is visible. After timer runs out, it is removed.
    unsigned int const c_StatusBarDelay = 5000;

    //! prevent pumping window because of scaling. Describes the expected size of the frame.
    int const c_extraPixelsBecauseOfFraming = 2;
} // namespace

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

    // connects for the usable push-buttons (and their hot keys)
    connect(ui->leftPB, &QPushButton::clicked, this, &CullendulaMainWindow::slotButtonLeftTriggered);
    connect(ui->rightPB, &QPushButton::clicked, this, &CullendulaMainWindow::slotButtonRightTriggered);
    connect(ui->savePB, &QPushButton::clicked, this, &CullendulaMainWindow::slotButtonSaveTriggered);
    connect(ui->trashPB, &QPushButton::clicked, this, &CullendulaMainWindow::slotButtonTrashTriggered);

    // necessary! even if the QLabel itself accepts already drops
    setAcceptDrops(true);

    // create the menu
    createActions();
    createMenus();

    // set undo/redo correctly
    updateUndoRedoButtonStatus();

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
        // set undo/redo correctly
        updateUndoRedoButtonStatus();
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
        // set undo/redo correctly
        updateUndoRedoButtonStatus();
    }
    else
    {
        qDebug() << "\tERROR: moving file did not succeed";
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::loadAndScalePhoto(QString const& path) const
{
    qDebug() << "CullendulaMainWindow::loadAndScalePhoto(): path=" << path;

    QPixmap const pixmap = QPixmap(path);

    // scale while keeping the aspect ratio
    int width = ui->centerLabel->width() - c_extraPixelsBecauseOfFraming;
    int height = ui->centerLabel->height() - c_extraPixelsBecauseOfFraming;
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

void CullendulaMainWindow::activateButtons(const bool active) const
{
    // en-/disable all four buttons
    ui->leftPB->setEnabled(active);
    ui->rightPB->setEnabled(active);
    ui->savePB->setEnabled(active);
    ui->trashPB->setEnabled(active);
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::printStatus(const QString & message) const
{
    // messages shall disappear after five seconds
    ui->statusBar->showMessage(message, c_StatusBarDelay);
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::createActions()
{
    // edit menu
    m_undoAction = new QAction(tr("Undo"), this);
    m_undoAction->setStatusTip(tr("Revert the last file-move-operation"));
    m_undoAction->setShortcut(Qt::CTRL | Qt::Key_Y);
    connect(m_undoAction, &QAction::triggered, this, [=] () { qDebug("pressed Undo"); m_fileSystemHandler.undo(); updateUndoRedoButtonStatus(); });

    m_redoQtAction = new QAction(tr("Redo"), this);
    m_redoQtAction->setStatusTip(tr("Redo the last file-move-operation (means: undo undo)"));
    m_redoQtAction->setShortcut(Qt::CTRL | Qt::Key_Z);
    connect(m_redoQtAction, &QAction::triggered, this, [=] () { qDebug("pressed Redo"); m_fileSystemHandler.redo(); updateUndoRedoButtonStatus(); });

    // help menu
    m_aboutAction = new QAction(tr("About Cullendula"), this);
    m_aboutAction->setStatusTip(tr("Show the application's About box"));
    m_aboutAction->setShortcut(Qt::CTRL | Qt::Key_A);
    connect(m_aboutAction, &QAction::triggered, this, &CullendulaMainWindow::about);

    m_aboutQtAction = new QAction(tr("About Qt"), this);
    m_aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    m_aboutQtAction->setShortcut(Qt::CTRL | Qt::Key_Q);
    connect(m_aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(m_aboutQtAction, &QAction::triggered, this, [=] () { printStatus(tr("Invoked Help|About Qt")); }); // replaced the slot-call with a lambda :)
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::createMenus()
{
    // edit menu
    m_editMenu = menuBar()->addMenu(tr("Edit"));
    m_editMenu->addAction(m_undoAction);
    m_editMenu->addAction(m_redoQtAction);

    // help menu
    m_helpMenu = menuBar()->addMenu(tr("Help"));
    m_helpMenu->addAction(m_aboutAction);
    m_helpMenu->addAction(m_aboutQtAction);
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::updateUndoRedoButtonStatus()
{
    if(m_undoAction != nullptr)
    {
       m_undoAction->setEnabled(m_fileSystemHandler.canUndo());
    }

    if(m_redoQtAction != nullptr)
    {
       m_redoQtAction->setEnabled(m_fileSystemHandler.canRedo());
    }
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
                        "Source code can be found inside the repository at <a href='https://github.com/marcelpetrick/Cullendula/'>https://github.com/marcelpetrick/Cullendula</a><br>"
                        "Feel free to use and share: GPL v3 :3"));
    mBox.setTextFormat(Qt::RichText);
    mBox.exec();
}

//----------------------------------------------------------------------------
