//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

//own includes
#include "CullendulaFileSystemHandler.h"

// Qt includes
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QList>

//----------------------------------------------------------------------------

// for constants
namespace {
    //! Determines the name of the output-folders
    QString const c_hardcodedOutput = "output";
    QString const c_hardcodedTrash = "trash";
}

//----------------------------------------------------------------------------

CullendulaFileSystemHandler::CullendulaFileSystemHandler()
    :
      m_workingPath("")
{
    qDebug() << "CullendulaFileSystemHandler::CullendulaFileSystemHandler()";
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::setWorkingPath(const QString & urlPath)
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
    QString returnValue;
    qDebug() << "CullendulaFileSystemHandler::getCurrentImagePath():";

    // some defensive checks
    if(checkInternalSanity())
    {
        // check for existence
        QString const path(m_currentImages[m_positionCurrentFile].absoluteFilePath());
        qDebug() << "\tpath:" << path;
        QFile const tempFile(path);
        if(tempFile.exists()/* && tempFile.isReadable()*/) // latter is not the case with Ext4 here ..
        {
            returnValue = path;
        }
    }

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::adjustCurrentPositionBy(int const offset)
{
    bool const returnValue = checkInternalSanity();

    if(checkInternalSanity())
    {
        m_positionCurrentFile = (m_positionCurrentFile + m_currentImages.size() + offset) % m_currentImages.size();
    }

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::switchCurrentPositionToTheLeft()
{
    bool const returnValue = adjustCurrentPositionBy(-1);

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::switchCurrentPositionToTheRight()
{
    bool const returnValue = adjustCurrentPositionBy(1);

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::saveCurrentFile()
{
    bool returnValue(false);
    qDebug() << "CullendulaFileSystemHandler::saveCurrentFile():";

    //TODO check the returnValue!
    returnValue = moveCurrentFileToGivenSubfolder(c_hardcodedOutput);

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::trashCurrentFile()
{
    bool returnValue(false);
    qDebug() << "CullendulaFileSystemHandler::trashCurrentFile():";

    //TODO check the returnValue!
    returnValue = moveCurrentFileToGivenSubfolder(c_hardcodedTrash);

    return returnValue;
}

//----------------------------------------------------------------------------

QString CullendulaFileSystemHandler::getCurrentStatus() const
{
    auto returnValue = QString("showing %1 of %2").arg(
                QString::number(m_positionCurrentFile + 1), //for the regular users indexing starts at 1 ..
                QString::number(m_currentImages.size()));

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::canUndo()
{
    return m_undoStack.canUndo();
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::canRedo()
{
    return m_undoStack.canRedo();
}

//----------------------------------------------------------------------------

void CullendulaFileSystemHandler::undo()
{
    if(canUndo())
    {
        qDebug() << "CullendulaFileSystemHandler::undo()"; //todom remove
        CullendulaUndoItem const item = m_undoStack.undo();
        // swap source & target
        QString const targetPath = item.sourcePath;
        QString const sourcePath = item.targetPath;
        qDebug() << "\tsource: " << sourcePath; //todom remove
        qDebug() << "\ttarget: " << targetPath; //todom remove
        QDir fileHandler;
        bool const successfullyRenamed = fileHandler.rename(sourcePath, targetPath);
        qDebug() << "rename was: " << (successfullyRenamed ? "TRUE" : "ERROR"); //todom remove

        // handle error-case (could fail for at least one filesystem)
        if(successfullyRenamed)
        {
            // update the file-list (means: rescan?)
            qDebug() << "update the file-list (means: rescan?)"; //todom remove

            //! @todo something like createImageFileList() has to be called, but without modiyfing the current position and re-initialisation of the filter ..
        }
    }
}

//----------------------------------------------------------------------------

void CullendulaFileSystemHandler::redo()
{
    if(canRedo())
    {
        qDebug() << "CullendulaFileSystemHandler::redo()"; //todom remove
        CullendulaUndoItem const item = m_undoStack.redo();
        // swap source & target
        QString const targetPath = item.sourcePath;
        QString const sourcePath = item.targetPath;
        qDebug() << "\tsource: " << sourcePath; //todom remove
        qDebug() << "\ttarget: " << targetPath; //todom remove
        QDir fileHandler;
        bool const successfullyRenamed = fileHandler.rename(sourcePath, targetPath);
        qDebug() << "rename was: " << (successfullyRenamed ? "TRUE" : "ERROR"); //todom remove

        // handle error-case (could fail for at least one filesystem)
        if(successfullyRenamed)
        {
            // update the file-list (means: rescan?)
            qDebug() << "update the file-list (means: rescan?)"; //todom remove

            //! @todo something like createImageFileList() has to be called, but without modiyfing the current position and re-initialisation of the filter ..
        }
    }
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
    QString const intermediatePath =
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

    // additionally check if the directory is usable
    if(tempDir.exists())
    {
        m_workingPath.setPath(tempDir.path());

        // trigger now the creation of the parsing
        returnValue = createImageFileList();

        // create now the output-folders
        returnValue &= createOutputFolder(c_hardcodedOutput);
        returnValue &= createOutputFolder(c_hardcodedTrash);
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
    QStringList filters; //! @TODO make the filter configurable for the user
    filters << "*.jpg" << "*.jpeg";
    m_workingPath.setNameFilters(filters);

    QFileInfoList const availableImages = m_workingPath.entryInfoList(QDir::Files);

    if(!availableImages.isEmpty())
    {
        // just for debugging: could be removed
        qDebug() << "found the following JPGs:";
        for(auto const& file : availableImages)
        {
            qDebug() << "\t" << file.absoluteFilePath();
        }

        // save the current file-list as vector and initialize the position-member
        m_currentImages = availableImages.toVector();
        m_positionCurrentFile = 0;

        returnValue = true; // everything ok
    }
    else
    {
        qDebug() << "no nice files found :(";
    }

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::createOutputFolder(QString const & subdir)
{
    bool returnValue(false);

    qDebug() << "CullendulaFileSystemHandler::createOutputFolder():" << subdir;

    QDir outputDirTest(m_workingPath.path() + QDir::separator() + subdir);

    if(outputDirTest.exists())
    {
        qDebug() << "output-folder exists already :) - nothing to do";
        returnValue = true;
    }
    else
    {
        //create a new output dir
        bool const creationSuccessful = m_workingPath.mkdir(subdir);
        Q_UNUSED(creationSuccessful) // return-value is not evaluated, because the next check test directly for existence of the directory

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

bool CullendulaFileSystemHandler::moveCurrentFileToGivenSubfolder(QString const & subdir)
{
    bool returnValue(false);
    qDebug() << "CullendulaFileSystemHandler::moveCurrentFileToGivenSubfolder():";

    bool const saneInternalState = checkInternalSanity();

    if(saneInternalState)
    {
        // move the current file to the output-folder
        QDir outputDir(m_workingPath.path() + QDir::separator() + subdir);
        if(outputDir.exists())
        {
            QString const sourcePathAndName(m_currentImages[m_positionCurrentFile].absoluteFilePath()); // TODO this will lead to crash, because index out of bounds
            qDebug() << "\t sourcePathAndName:" << sourcePathAndName;
            QFileInfo fileInfo(sourcePathAndName);
            QString const fileName(fileInfo.fileName());
            //qDebug() << "\t fileName:" << fileName;
            QString const targetPathAndName(outputDir.path() + QDir::separator() + fileName);
            qDebug() << "\t targetPathAndName:" << targetPathAndName;
            bool const successfullyRenamed = outputDir.rename(sourcePathAndName, targetPathAndName); //! this is the MOVE operation!
            qDebug() << "\t successfullyRenamed?" << successfullyRenamed;

            // in case the file was successfully moved (= renamed path), then adjust the internal structures
            if(successfullyRenamed)
            {
                //! add to the stack for possible undoing
                m_undoStack.push(sourcePathAndName, targetPathAndName);

                // go to the next picture by removing the entry from the file-list, but keep the position
                m_currentImages.removeAt(m_positionCurrentFile);
                //if this was the last item of the list (like pos 2 at list of 3; which has now just 2 elements), then modulo
                int const listSize = m_currentImages.size();
                m_positionCurrentFile = (listSize > 0) ? (m_positionCurrentFile % listSize) : -1;
                returnValue = true;
            }
        }
    }

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::checkInternalSanity() const
{
    bool returnValue(true);

    QDir const outputDir(m_workingPath.path());
    if(!outputDir.exists())
    {
        qDebug() << "CullendulaFileSystemHandler::checkInternalSanity(): ERROR: workingPath not valid!";
        returnValue = false;
    }

    if(m_currentImages.empty())
    {
        qDebug() << "CullendulaFileSystemHandler::checkInternalSanity(): ERROR: list of current images is empty!";
        returnValue = false;
    }

    if(m_positionCurrentFile < 0)
    {
        qDebug() << "CullendulaFileSystemHandler::checkInternalSanity(): ERROR: position"
                 << QString::number(m_positionCurrentFile) << "is a negative number!";
        returnValue = false;
    }

    if(m_positionCurrentFile >= m_currentImages.size())
    {
        qDebug() << "CullendulaFileSystemHandler::checkInternalSanity(): ERROR: position"
                 << QString::number(m_positionCurrentFile) << " is out of range of the image-list with size" << QString::number(m_currentImages.size());
        returnValue = false;
    }

    return returnValue;
}
