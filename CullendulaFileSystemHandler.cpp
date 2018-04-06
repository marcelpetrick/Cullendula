//own includes
#include "CullendulaFileSystemHandler.h"

// Qt includes
#include <QtCore/QList>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QDebug> //todom maybe remove

CullendulaFileSystemHandler::CullendulaFileSystemHandler()
    :
    m_workingPath("")
{
    qDebug() << "CullendulaFileSystemHandler::CullendulaFileSystemHandler()";
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::setWorkingPath(const QString urlPath)
{
    bool returnValue(false);

    qDebug() << "CullendulaFileSystemHandler::setWorkingPath(): urlPath=" << urlPath;
    m_workingPath.setPath("");
    m_workingPath.setPath(urlPath);

    // trigger now the follow-up
    returnValue = processNewPath();

    return returnValue;
}

//----------------------------------------------------------------------------

QString CullendulaFileSystemHandler::getCurrentImagePath()
{
    qDebug() << "CullendulaFileSystemHandler::getCurrentImagePath():";

    // some defensive checks: so is at least one file available?
    if(m_currentImages.isEmpty())
    {
        qDebug() << "ERROR: invalid (empty) file-list! will return ... but then the text-label shall appear again";
        return QString();
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

    return path;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::switchCurrentPositionToTheLeft()
{
    bool returnValue(false);
    m_positionCurrentFile = (m_positionCurrentFile + m_currentImages.size() - 1) % m_currentImages.size();
    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::switchCurrentPositionToTheRight()
{
    bool returnValue(false);
    m_positionCurrentFile = (m_positionCurrentFile + 1) % m_currentImages.size();
    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::saveCurrentFile()
{
    bool returnValue(false);
    qDebug() << "CullendulaFileSystemHandler::saveCurrentFile():";

    //TODO check the returnValue!
    moveCurrentFileToGivenSubfolder(c_hardcodedOutput);

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::trashCurrentFile()
{
    bool returnValue(false);
    qDebug() << "CullendulaFileSystemHandler::trashCurrentFile():";

    //TODO check the returnValue!
    moveCurrentFileToGivenSubfolder(c_hardcodedTrash);

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::processNewPath()
{
    bool returnValue(false);

    qDebug() << "CullendulaFileSystemHandler::processNewPath():";

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
        returnValue = createImageFileList();

        // create now the output-folder
        returnValue &= createOutputFolder();
    }
    else
    {
        qDebug() << "ERROR: given directory does not exist";
    }

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::createImageFileList()
{
    bool returnValue(false);

    qDebug() << "CullendulaFileSystemHandler::createImageFileList():";

    // apply the wanted filters
    QStringList filters; // TODO make configureable for the user
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
        //refreshLabel(); // TODO originally before
        returnValue = true; // everything ok
    }
    else
    {
        qDebug() << "no nice files found :(";
    }

    return returnValue;
}


//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::createOutputFolder()
{
    bool returnValue(false);

    qDebug() << "CullendulaFileSystemHandler::createOutputFolder():";

    QDir outputDirTest(m_workingPath.path() + QDir::separator() + c_hardcodedOutput);

    if(outputDirTest.exists())
    {
        qDebug() << "output-folder exists already :) - nothing to do";
    }
    else
    {
        //create a new output dir
        m_workingPath.mkdir(c_hardcodedOutput);
        if(outputDirTest.exists())
        {
            qDebug() << "output-folder exists after creation!";
            returnValue = true;
        }
        else
        {
            qDebug() << "very severe error - could not create output-dir :(";
        }
    }

    // TODO maybe return something or throw or whatever ... delete /home/..
        return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::moveCurrentFileToGivenSubfolder(QString const subdir)
{
    bool returnValue(false);
    qDebug() << "CullendulaFileSystemHandler::moveCurrentFileToGivenSubfolder():";

    // move the current file to the output-folder
    QDir outputDir(m_workingPath.path() + QDir::separator() + subdir); // TODO maybe save as member - equal to 'outputDirTest'
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
            returnValue = true;
        }
    }

    return returnValue;
}

