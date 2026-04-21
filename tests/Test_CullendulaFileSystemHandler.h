//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#pragma once

#include <QtTest/QtTest>
#include <memory>

#include "CullendulaFileSystemHandler.h"

/*!
 * @file
 * @brief QTest suite for CullendulaFileSystemHandler.
 */

/*!
 * @brief Exercises file discovery, navigation, filtering, and move semantics.
 */
class Test_CullendulaFileSystemHandler : public QObject {
    Q_OBJECT

   private Q_SLOTS:
    //! Create a fresh temporary workspace and handler before each test.
    void init();

    //! Destroy test-owned resources after each test.
    void cleanup();

    //! Verify that loading a directory path discovers matching images.
    void slot_Test_SetWorkingPath_FromDirectory();

    //! Verify that loading a file path resolves its parent directory.
    void slot_Test_SetWorkingPath_FromImageFile();

    //! Verify normalization and upper bound of suggested extensions.
    void slot_Test_GetSuggestedImageExtensions_IsNormalizedAndBounded();

    //! Verify invalid extension entries are removed during normalization.
    void slot_Test_SetAllowedImageExtensions_NormalizesAndDropsUnsupportedValues();

    //! Verify extension filters are respected during directory scans.
    void slot_Test_SetAllowedImageExtensions_FiltersFiles();

    //! Verify PNG files and uppercase suffixes are accepted when supported.
    void slot_Test_SetWorkingPath_FindsPngAndUppercaseSuffixes();

    //! Verify invalid paths fail cleanly.
    void slot_Test_SetWorkingPath_InvalidPath();

    //! Verify empty directories do not create a usable image session.
    void slot_Test_SetWorkingPath_NoImages();

    //! Verify failed reloads clear the previous in-memory state.
    void slot_Test_SetWorkingPath_ClearsStateOnReloadFailure();

    //! Verify navigation wraps around at both ends of the list.
    void slot_Test_NavigationWrapsAround();

    //! Verify saving moves the file and advances the session state.
    void slot_Test_SaveCurrentFile_MovesFileAndUpdatesState();

    //! Verify trashing moves the file and advances the session state.
    void slot_Test_TrashCurrentFile_MovesFileAndUpdatesState();

    //! Verify undo and redo rename the expected files on disk.
    void slot_Test_UndoRedo_MoveFilesOnDisk();

    //! Verify save fails when the destination file already exists.
    void slot_Test_SaveCurrentFile_FailsOnNameCollision();

   private:
    /*!
     * @brief Create a test file below the temporary workspace.
     * @param relativePath Relative path below the temporary directory.
     * @return Absolute file path of the created file.
     */
    QString createFile(QString const& relativePath);

    //! Create a representative image set used by multiple test cases.
    void createImageSet();

    //! Temporary root directory for filesystem-based tests.
    std::unique_ptr<QTemporaryDir> m_tempDir;

    //! Handler instance under test.
    std::unique_ptr<CullendulaFileSystemHandler> m_handler;
};
