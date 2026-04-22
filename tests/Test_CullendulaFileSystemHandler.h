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

    //! Verify suggested extensions append additional supported formats when preferred ones do not fill the limit.
    void slot_Test_GetSuggestedImageExtensions_AppendsAdditionalSupportedFormats();

    //! Verify suggested extensions stop appending once the helper reaches its hard cap.
    void slot_Test_GetSuggestedImageExtensions_StopsWhenAdditionalFormatsReachLimit();

    //! Verify invalid extension entries are removed during normalization.
    void slot_Test_SetAllowedImageExtensions_NormalizesAndDropsUnsupportedValues();

    //! Verify extension filters are respected during directory scans.
    void slot_Test_SetAllowedImageExtensions_FiltersFiles();

    //! Verify PNG files and uppercase suffixes are accepted when supported.
    void slot_Test_SetWorkingPath_FindsPngAndUppercaseSuffixes();

    //! Verify invalid paths fail cleanly.
    void slot_Test_SetWorkingPath_InvalidPath();

    //! Verify paths whose parent directory does not exist fail with an actionable error.
    void slot_Test_SetWorkingPath_PathWithMissingParentDirectory_ReturnsError();

    //! Verify setup fails when the output directory path is blocked by a file.
    void slot_Test_SetWorkingPath_FailsWhenOutputDirectoryCannotBeCreated();

    //! Verify setup fails when the trash directory path is blocked by a file.
    void slot_Test_SetWorkingPath_FailsWhenTrashDirectoryCannotBeCreated();

    //! Verify save recreates the output directory if it was deleted after load.
    void slot_Test_SaveCurrentFile_RecreatesMissingOutputDirectory();

    //! Verify trash recreates the trash directory if it was deleted after load.
    void slot_Test_TrashCurrentFile_RecreatesMissingTrashDirectory();

    //! Verify save fails with a detailed error when the output path is blocked after load.
    void slot_Test_SaveCurrentFile_FailsWhenOutputDirectoryPathIsBlockedAfterLoad();

    //! Verify empty directories do not create a usable image session.
    void slot_Test_SetWorkingPath_NoImages();

    //! Verify failed reloads clear the previous in-memory state.
    void slot_Test_SetWorkingPath_ClearsStateOnReloadFailure();

    //! Verify missing current files are reported as no current image path.
    void slot_Test_GetCurrentImagePath_ReturnsEmptyWhenCurrentFileWasDeleted();

    //! Verify navigation wraps around at both ends of the list.
    void slot_Test_NavigationWrapsAround();

    //! Verify saving moves the file and advances the session state.
    void slot_Test_SaveCurrentFile_MovesFileAndUpdatesState();

    //! Verify saving the final remaining image clears the current selection.
    void slot_Test_SaveCurrentFile_LastImageClearsCurrentSelection();

    //! Verify trashing moves the file and advances the session state.
    void slot_Test_TrashCurrentFile_MovesFileAndUpdatesState();

    //! Verify undo and redo rename the expected files on disk.
    void slot_Test_UndoRedo_MoveFilesOnDisk();

    //! Verify save resolves destination collisions through a unique target name.
    void slot_Test_SaveCurrentFile_UsesUniqueNameOnCollision();

    //! Verify save skips over several occupied collision suffixes.
    void slot_Test_SaveCurrentFile_UsesNextFreeCollisionSuffix();

    //! Verify moving without a loaded session returns a user-facing error.
    void slot_Test_SaveCurrentFile_WithoutLoadedImage_ReturnsError();

    //! Verify failed filesystem renames surface actionable save errors.
    void slot_Test_SaveCurrentFile_RenameFailure_ReturnsError();

    //! Verify trash resolves destination collisions through a unique target name.
    void slot_Test_TrashCurrentFile_UsesUniqueNameOnCollision();

    //! Verify failed filesystem renames surface actionable trash errors.
    void slot_Test_TrashCurrentFile_RenameFailure_ReturnsError();

    //! Verify undo without history returns a user-facing error.
    void slot_Test_Undo_WithoutHistory_ReturnsError();

    //! Verify redo without history returns a user-facing error.
    void slot_Test_Redo_WithoutHistory_ReturnsError();

    //! Verify undo failures are reported when the moved file disappeared.
    void slot_Test_Undo_WhenMovedFileIsMissing_ReturnsFalse();

    //! Verify failed undo operations keep history intact for a retry.
    void slot_Test_Undo_FailurePreservesUndoAndRedoHistory();

    //! Verify redo failures are reported when the restored file disappeared.
    void slot_Test_Redo_WhenRestoredFileIsMissing_ReturnsFalse();

    //! Verify failed redo operations keep history intact for a retry.
    void slot_Test_Redo_FailurePreservesUndoAndRedoHistory();

    //! Verify rebuilding the image list falls back when a preferred image path is gone.
    void slot_Test_RebuildImageFileList_MissingPreferredPathUsesFallbackPosition();

    //! Verify redo keeps the current fallback position when the restored source is no longer listed.
    void slot_Test_Redo_MissingSourceIndexUsesCurrentFallbackPosition();

    //! Verify createOutputFolder reports mkdir failures through the helper seam.
    void slot_Test_CreateOutputFolder_HelperReportsMkdirFailure();

    //! Verify createOutputFolder reports missing directories after a reported creation success.
    void slot_Test_CreateOutputFolder_HelperReportsMissingDirectoryAfterCreation();

    //! Verify internal sanity rejects an out-of-range current image index.
    void slot_Test_CheckInternalSanity_FailsForOutOfRangeIndex();

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
