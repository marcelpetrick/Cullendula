// own includes
#include "CullendulaMainWindow.h"
#include "ui_CullendulaMainWindow.h"

// Qt includes
#include <QtGui/QDropEvent>
#include <QtGui/QPixmap>

#include <QtCore/QMimeData>
#include <QtCore/QList>
#include <QtCore/QDebug> //todom maybe remove
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

//----------------------------------------------------------------------------

CullendulaMainWindow::CullendulaMainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CullendulaMainWindow),
    m_workingPath("")
{
    setAcceptDrops(true);
    ui->setupUi(this);
    // present the picture fully visible .. TODO has to be reworked later
    //ui->centerLabel->setScaledContents(true);

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
            m_workingPath.setPath("");
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
    qDebug() << "CullendulaMainWindow::slotButtonLeftTriggered()";

    m_positionCurrentFile = (m_positionCurrentFile + m_currentImages.size() - 1) % m_currentImages.size();
    refreshLabel();
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::slotButtonRightTriggered()
{
    qDebug() << "CullendulaMainWindow::slotButtonRightTriggered()";

    m_positionCurrentFile = (m_positionCurrentFile + 1) % m_currentImages.size();
    refreshLabel();
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::slotButtonCenterTriggered()
{
    qDebug() << "CullendulaMainWindow::slotButtonCenterTriggered()";

    // move the current file to the output-folder
    QDir outputDir(m_workingPath.path() + QDir::separator() + c_hardcodedOutput); // TODO maybe save as member - equal to 'outputDirTest'
    if(outputDir.exists())
    {
        QString const path(m_currentImages[m_positionCurrentFile].absoluteFilePath()); // TODO this will lead to crash, because index out of bounds
        qDebug() << "\t path of file to move:" << path;
        QFileInfo fileInfo(path);
        QString const fileName (fileInfo.fileName());
        qDebug() << "fileName:" << fileName;
        QString const outputFileName(outputDir.path() + QDir::separator() + fileName);
        qDebug() << "outputFileName:" << outputFileName;
        bool const successfullyRenamed = outputDir.rename(path, outputFileName);
        qDebug() << "successfullyRenamed?" << successfullyRenamed;

        if(successfullyRenamed)
        {
            // go to the next picture by removing the entry from the file-list, but keep the position
            m_currentImages.removeAt(m_positionCurrentFile);
            refreshLabel();
        }
    }
    else
    {
        qDebug() << "output folder does not exist!";
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::processNewPath()
{
    qDebug() << "CullendulaMainWindow::processNewPath():";

    // convert the given path (which maybe includes a filename)
//    qDebug() << "fileInfo gets the following path:" << m_workingPath.path();

    // problem: windows shows as /c:/dir/file.suffix - linux here as /home/dir/file.suffix
    // so cut the first character for win, but don't for linux ..
    QString intermediatePath =
#ifdef __linux__
            m_workingPath.path();
#else
            m_workingPath.path().remove(0, 1); // remove the leading slash ("/")
#endif
    QFileInfo const fileInfo(intermediatePath);

    //! trim the path
    //! shall return for \Cullendula\testItemFolder --> \Cullendula\testItemFolder
    //! and for \Cullendula\testItemFolder\cat0.jpg --> \Cullendula\testItemFolder
//    qDebug() << "\tfileInfo.isFile():" << fileInfo.isFile();
//    qDebug() << "fileInfo.path(): " << fileInfo.path();
//    qDebug() << "fileInfo.filePath(): " << fileInfo.filePath();
//    qDebug() << "fileInfo.canonicalFilePath(): " << fileInfo.canonicalFilePath();
    qDebug() << "\tfileInfo.isDir():" << fileInfo.isDir();
    qDebug() << "fileInfo.absoluteFilePath(): " << fileInfo.absoluteFilePath();
    qDebug() << "fileInfo.absolutePath(): " << fileInfo.absolutePath();

    //    qDebug() << "fileInfo.fileName(): " << fileInfo.fileName();
    QDir const tempDir = QDir(fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.absolutePath());
    qDebug() << "\t resulting directory:" << tempDir.path();

    // additionally check if the directory is useable
    if(tempDir.exists())
    {
        m_workingPath.setPath(tempDir.path());

        // trigger now the creation of the parsing
        createImageFileList();

        // create now the output-folder
        createOutputFolder();
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
        qDebug() << "ERROR: invalid (empty) file-list! will return ... but then the text-label shall appear again";
        return;
    }

    if(m_positionCurrentFile < 0)
    {
        qDebug() << "WARNING: invalid position given: current < 0";
        m_positionCurrentFile = 0;
    }

    if(m_positionCurrentFile >= m_currentImages.size())
    {
        qDebug() << "WARNING: invalid position given: current > number of files";
        m_positionCurrentFile = m_currentImages.size() - 1;
    }

    // TODO check for existance
    QString const path(m_currentImages[m_positionCurrentFile].absoluteFilePath());
    qDebug() << "\t" << path;

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
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::createOutputFolder()
{
    qDebug() << "CullendulaMainWindow::createOutputFolder():";

    QDir outputDirTest(m_workingPath.path() + QDir::separator() + c_hardcodedOutput);

    if(outputDirTest.exists())
    {
        qDebug() << "output-folder exists already :) - nothing to do";
    }
    else
    {
        //create a new output dir
        m_workingPath.mkdir("output");
        if(outputDirTest.exists())
        {
            qDebug() << "output-folder exists after creation!";
        }
        else
        {
            qDebug() << "very severe error - could not create output-dir :(";
        }
    }

    // TODO maybe return something or throw or whatever ... delete /home/..
}
