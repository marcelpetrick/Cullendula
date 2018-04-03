
// own includes
#include "CullendulaMainWindow.h"
#include "ui_CullendulaMainWindow.h"

// Qt includes
#include <QtGui/QDropEvent>
#include <QtCore/QMimeData>
#include <QtCore/QList>
#include <QtCore/QDebug> //todom maybe remove
#include <QFileInfo>
#include <QPixmap>
#include <QFile>

//----------------------------------------------------------------------------

CullendulaMainWindow::CullendulaMainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CullendulaMainWindow),
    m_workingPath("")
{
    setAcceptDrops(true);
    ui->setupUi(this);
    // present the picture fully visible .. TODO has to be reworked later
    ui->centerLabel->setScaledContents(true);

    // connects for the useable push-buttons (and their hotkeys)
    connect(ui->leftPB, &QPushButton::clicked, this, &CullendulaMainWindow::slotButtonLeftTriggered);
    connect(ui->rightPB, &QPushButton::clicked, this, &CullendulaMainWindow::slotButtonRightTriggered);
    connect(ui->savePB, &QPushButton::clicked, this, &CullendulaMainWindow::slotButtonCenterTriggered);

    // necessary! even if the QLabel itself accepts already drops
    setAcceptDrops(true);
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
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::dragMoveEvent(QDragMoveEvent* event)
{
    Q_UNUSED(event)
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::dragLeaveEvent(QDragLeaveEvent* event)
{
    Q_UNUSED(event)
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();

    // check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls())
    {
        QStringList pathList;
        QList<QUrl> urlList = mimeData->urls();

        // extract the local paths of the files: print all of them; can be removed laters
        qDebug() << "new drop:";
        for (auto const& url : urlList)
        {
            qDebug() << "\tdropped: " << url;
        }

        // just use the very first one ..
        if(!urlList.isEmpty())
        {
            m_workingPath.setPath(urlList.first().path());

            // trigger now the follow-up
            processNewPath();
        }
    }

    event->acceptProposedAction();
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::slotButtonLeftTriggered()
{
    qDebug() << "void CullendulaMainWindow::slotButtonLeftTriggered()";

    // TODO just for testing if the shortcuts work
    m_positionCurrentFile = (m_positionCurrentFile + m_currentImages.size() - 1) % m_currentImages.size();
    refreshLabel();
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::slotButtonRightTriggered()
{
    qDebug() << "void CullendulaMainWindow::slotButtonRightTriggered()";

    m_positionCurrentFile = (m_positionCurrentFile + 1) % m_currentImages.size();
    refreshLabel();
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::slotButtonCenterTriggered()
{
    qDebug() << "void CullendulaMainWindow::slotButtonCenterTriggered()";
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::processNewPath()
{
    qDebug() << "CullendulaMainWindow::processNewPath():";

    // convert the given path (which maybe includes a filename)
//    qDebug() << "fileInfo gets the following path:" << m_workingPath.path();
    QFileInfo const fileInfo(m_workingPath.path().remove(0, 1)); // remove the leading /

    //! trim the path
    //! shall return for \Cullendula\testItemFolder --> \Cullendula\testItemFolder
    //! and for \Cullendula\testItemFolder\cat0.jpg --> \Cullendula\testItemFolder
//    qDebug() << "\tfileInfo.isFile():" << fileInfo.isFile();
//    qDebug() << "fileInfo.path(): " << fileInfo.path();
//    qDebug() << "fileInfo.filePath(): " << fileInfo.filePath();
//    qDebug() << "fileInfo.canonicalFilePath(): " << fileInfo.canonicalFilePath();
//    qDebug() << "fileInfo.absoluteFilePath(): " << fileInfo.absoluteFilePath();

//    qDebug() << "fileInfo.fileName(): " << fileInfo.fileName();
    QDir const tempDir = QDir(fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.absolutePath());
    qDebug() << "\t resulting directory:" << tempDir.path();

    // additionally check if the directory is useable
    if(tempDir.exists())
    {
        m_workingPath.setPath(tempDir.path());

        // trigger now the creation of the parsing
        createImageFileList();
    }
    else
    {
        qDebug() << "ERROR: given directory does not exist";
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::createImageFileList()
{
    qDebug() << "CullendulaMainWindow::createImageFileList():";

    // apply the wanted filters
    QStringList filters;
    filters << "*.jpg" << "*.jpeg";
    m_workingPath.setNameFilters(filters);

    QFileInfoList const availableImages = m_workingPath.entryInfoList(QDir::Files);

    if(!availableImages.isEmpty())
    {
        qDebug() << "found the following JPGs:";
        for(auto const& file : availableImages)
        {
            qDebug() << "\t" << file.absoluteFilePath();
        }

//! attention: just for testing!
//        // load now the very first image
//        QString const path = availableImages.first().absoluteFilePath();
//        qDebug() << "file to use:" << path;
//        // load image
//        QPixmap const pixmap = QPixmap(path);
//        // set to the label
//        ui->centerLabel->setScaledContents(true);
//        ui->centerLabel->setPixmap(pixmap);
//! end of testing

        // save the current file-list as vector and initialize the position-member
        m_currentImages = availableImages.toVector();
        m_positionCurrentFile = 0;

        // trigger the initial loading
        refreshLabel();
    }
    else
    {
        qDebug() << "no nice files found :(";
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::refreshLabel()
{
    qDebug() << "CullendulaMainWindow::refreshLabel():";

    // some defensive checks
    if(m_currentImages.isEmpty())
    {
        qDebug() << "ERROR: invalid file-list!";
    }

    if(m_positionCurrentFile < 0 || m_positionCurrentFile >= m_currentImages.size())
    {
        qDebug() << "ERROR: invalid given position";
    }

    //check for existance
    QString const path(m_currentImages[m_positionCurrentFile].absoluteFilePath());
    qDebug() << path;
    //QFile currentFile = QFile(path);

    QPixmap const pixmap = QPixmap(path);
    // set to the label

    ui->centerLabel->setPixmap(pixmap);
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::createOutputFolder()
{
    qDebug() << "CullendulaMainWindow::createOutputFolder():";

    // TODO
}
