#pragma once

// Qt includes
#include <QtWidgets/QMainWindow>
#include <QObject>
#include <QDir>
#include <QFileInfo>

// for constants
namespace {
    QString const c_hardcodedOutput = "output";
    QString const c_hardcodedTrash = "trash";
}

class CullendulaFileSystemHandler
{
public:
    CullendulaFileSystemHandler();

    // [methods]

    //! TODO
    bool setWorkingPath(QString const urlPath);

    //! TODO
    QString getCurrentImagePath();

    //! TODO
    bool switchCurrentPositionToTheLeft();

    //! TODO
    bool switchCurrentPositionToTheRight();

    //! TODO
    bool saveCurrentFile();

    //! TODO
    bool trashCurrentFile();

private:
    //! checks the currently set path and gets the file-list. Afterwards triggers loading of the current image.
    bool processNewPath();

    //! Scan given path for image-file-types (for now suffix: jpg, jpeg - more later)
    bool createImageFileList();

    //! Create and check if an output-folder exists.
    //! Initially: just create sub-dir "output".
    bool createOutputFolder();

    //! TODO
    bool moveCurrentFileToGivenSubfolder(QString const subdir);

    // [members]
    // the currently chosen path to the directory (not file)
    QDir m_workingPath;

    // saves the current state of the given directory; initially empty; shall be reset(TODO)
    QVector<QFileInfo> m_currentImages;
    int m_positionCurrentFile = -1;
};
