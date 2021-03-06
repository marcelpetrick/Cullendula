//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#pragma once

// own includes
#include "CullendulaUndoStack.h"

// Qt includes
#include <QtCore/QVector>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

//----------------------------------------------------------------------------

class CullendulaFileSystemHandler
{
public:
    CullendulaFileSystemHandler();

    //! Allows to configure the current path for the search for the album.
    bool setWorkingPath(QString const & urlPath);

    //! Return the path to the current item.
    //! In case no path is valid: return empty QString.
    QString getCurrentImagePath();

    //! Move to the previous (left) picture-file in the vector.
    bool switchCurrentPositionToTheLeft();

    //! Move to the next (right) picture-file in the vector.
    bool switchCurrentPositionToTheRight();

    //! Moves the current file to the "output"-folder.
    bool saveCurrentFile();

    //! Moves the current file to the "trash"-folder.
    bool trashCurrentFile();

    //! Return a string which describes the current position in the file-list.
    QString getCurrentStatus() const;

    //! Checks if the option is possible. Useful for the GUI (dis-/enabled).
    //! Hands over the functionality from the CullendulaUndoStack.
    bool canUndo();
    bool canRedo();

    //! Execute the undo.
    void undo();

    //! Execute the redo.
    void redo();

private:
    //! checks the currently set path and gets the file-list. Afterwards triggers loading of the current image.
    bool processNewPath();

    //! Scan given path for image-file-types (for now suffix: jpg, jpeg - more later)
    bool createImageFileList();

    //! Create and check if an output-folder exists.
    //! Initially: just create sub-dir "output".
    bool createOutputFolder(QString const & subdir);

    //! Unified functionality to move the current file. Called by save/trash.
    bool moveCurrentFileToGivenSubfolder(QString const & subdir);

    //! Check if the current file-list, position and working-path are valid. Else return false.
    bool checkInternalSanity() const;

    //! adjust the current position inside the file list by the given offset.
    //! @returns sanity-check
    bool adjustCurrentPositionBy(const int offset);

    // [members]
    //! the currently chosen path to the directory (not file)
    QDir m_workingPath;

    //! saves the current state of the given directory; initially empty; shall be reset (TODO)
    QVector<QFileInfo> m_currentImages;
    int m_positionCurrentFile = -1;

    //! Stores the last actions for Undo/Redo
    CullendulaUndoStack m_undoStack;
};
