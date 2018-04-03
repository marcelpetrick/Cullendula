
// own includes
#include "CullendulaMainWindow.h"
#include "ui_CullendulaMainWindow.h"

// Qt includes
#include <QtGui/QDropEvent>
#include <QtCore/QMimeData>
#include <QtCore/QList>
#include <QtCore/QDebug> //todom maybe remove
#include <QFileInfo>

//----------------------------------------------------------------------------

CullendulaMainWindow::CullendulaMainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CullendulaMainWindow),
    m_workingPath("")
{
    setAcceptDrops(true);
    ui->setupUi(this);

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

    QFileInfoList availableImages = m_workingPath.entryInfoList(QDir::Files);

    if(!availableImages.isEmpty())
    {
        qDebug() << "found the following JPGs:";
        for(auto const& file : availableImages)
        {
            qDebug() << "\t" << file.absoluteFilePath();
        }
    }
    else
    {
        qDebug() << "no nice files found :(";
    }

    // TODO load the picture and apply as  pixmap to the label
    //ui->label->setPixmap(QPixmap(availableImages.first().path()));
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::createOutputFolder()
{
    qDebug() << "CullendulaMainWindow::createOutputFolder():";

    // TODO
}
