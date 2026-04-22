//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#pragma once

// own includes
#include "CullendulaUndoStack.h"

// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QSet>
#include <QtCore/QStringList>
#include <QtCore/QVector>

class Test_CullendulaFileSystemHandler;

//----------------------------------------------------------------------------

/*!
 * @file
 * @brief File-system facing application logic for browsing and moving images.
 */

/*!
 * @brief Manages the current image session for one working directory.
 *
 * The handler scans a directory for supported image files, keeps track of the
 * currently selected image, exposes wrap-around navigation, and moves images
 * into dedicated `output` or `trash` subdirectories. It also maintains the
 * corresponding undo/redo history for those move operations.
 */
class CullendulaFileSystemHandler {
    Q_DECLARE_TR_FUNCTIONS(CullendulaFileSystemHandler)

   public:
    //! Construct an empty handler with the default set of suggested extensions.
    CullendulaFileSystemHandler();

    /*!
     * @brief Return the preferred image suffixes supported by the current Qt setup.
     * @return Up to ten normalized, lowercase suffixes ordered by application
     *         preference and filtered against the active Qt image plugins.
     */
    static QStringList getSuggestedImageExtensions();

    /*!
     * @brief Set the image suffixes accepted for the next directory scan.
     * @param extensions Candidate suffixes without the leading dot.
     *
     * Unsupported, empty, duplicate, and differently cased values are filtered
     * and normalized to lowercase before being stored.
     */
    void setAllowedImageExtensions(QStringList const& extensions);

    /*!
     * @brief Return the currently active image suffix filter.
     * @return Sorted list of normalized suffixes without a leading dot.
     */
    QStringList getAllowedImageExtensions() const;

    /*!
     * @brief Load a new working directory or a file inside that directory.
     * @param urlPath Path provided by the UI drop operation.
     * @return `true` when the directory could be resolved, scanned, and at least
     *         one matching image is available after applying the current filter.
     *
     * Passing a file path selects its parent directory. Any previous handler
     * state and undo history are cleared before the new path is processed.
     */
    bool setWorkingPath(QString const& urlPath);

    /*!
     * @brief Return the absolute path of the currently selected image.
     * @return Absolute file path when the internal state is valid and the file
     *         still exists on disk; otherwise an empty string.
     */
    QString getCurrentImagePath();

    /*!
     * @brief Select the previous image in the current list.
     * @return `true` when the handler contains a valid image list and the
     *         selection could be advanced with wrap-around semantics.
     */
    bool switchCurrentPositionToTheLeft();

    /*!
     * @brief Select the next image in the current list.
     * @return `true` when the handler contains a valid image list and the
     *         selection could be advanced with wrap-around semantics.
     */
    bool switchCurrentPositionToTheRight();

    /*!
     * @brief Move the current image into the `output` subdirectory.
     * @return `true` when the file move succeeded and the internal image list
     *         was updated accordingly.
     */
    bool saveCurrentFile();

    /*!
     * @brief Move the current image into the `trash` subdirectory.
     * @return `true` when the file move succeeded and the internal image list
     *         was updated accordingly.
     */
    bool trashCurrentFile();

    /*!
     * @brief Return a user-facing description of the current selection.
     * @return Status text in the form `showing X of Y`.
     */
    QString getCurrentStatus() const;

    /*!
     * @brief Return the most recent user-facing error message.
     * @return Empty string when no actionable error is pending.
     */
    QString const& getLastErrorMessage() const;

    /*!
     * @brief Check whether an undo step is currently available.
     * @return `true` when at least one move can be undone.
     */
    bool canUndo();

    /*!
     * @brief Check whether a redo step is currently available.
     * @return `true` when at least one move can be redone.
     */
    bool canRedo();

    /*!
     * @brief Undo the most recent move operation on disk.
     * @return `true` when the file rename succeeded and the in-memory image list
     *         was rebuilt successfully.
     */
    bool undo();

    /*!
     * @brief Redo the most recently undone move operation on disk.
     * @return `true` when the file rename succeeded and the in-memory image list
     *         was rebuilt successfully.
     */
    bool redo();

   private:
    friend class Test_CullendulaFileSystemHandler;

    //! Reset the cached image list, current index, and undo history.
    void resetCurrentState();

    /*!
     * @brief Resolve the configured path into a usable directory and scan it.
     * @return `true` when a directory exists, at least one matching image is
     *         found, and the auxiliary output folders are ready.
     */
    bool processNewPath();

    /*!
     * @brief Rebuild the image cache from the current working directory.
     * @return `true` when at least one matching image file was found.
     */
    bool createImageFileList();

    /*!
     * @brief Rebuild the cached image list while preserving the current selection.
     * @param preferredImagePath Absolute image path to keep selected when still present.
     * @param fallbackPosition Index to clamp to when the preferred image is gone.
     * @return `true` when the rebuilt list contains at least one matching image.
     */
    bool rebuildImageFileList(QString const& preferredImagePath, int fallbackPosition);

    /*!
     * @brief Ensure that a move target subdirectory exists below the working path.
     * @param subdir Name of the subdirectory, such as `output` or `trash`.
     * @return `true` when the directory exists after the call.
     *
     * On failure this stores a user-facing error that explains whether the
     * path is blocked by a non-directory entry or the directory creation
     * attempt itself failed.
     */
    bool createOutputFolder(QString const& subdir);

    /*!
     * @brief Move the current image into a dedicated subdirectory.
     * @param subdir Name of the destination subdirectory below the working path.
     * @return `true` when the file move succeeded and the undo stack plus image
     *         selection state were updated.
     *
     * The destination directory is validated and recreated on demand so manual
     * filesystem changes after the initial load are handled gracefully.
     */
    bool moveCurrentFileToGivenSubfolder(QString const& subdir);

    /*!
     * @brief Validate the internal working directory, image list, and selection.
     * @return `true` when the handler can safely access the current image.
     */
    bool checkInternalSanity() const;

    /*!
     * @brief Shift the current image selection by a signed offset.
     * @param offset Relative number of steps to move inside the image list.
     * @return `true` when the handler was in a valid state before the shift.
     */
    bool adjustCurrentPositionBy(const int offset);

    /*!
     * @brief Find the index of an image path inside the cached image list.
     * @param imagePath Absolute path to search for.
     * @return Zero-based index when the image is present, otherwise `-1`.
     */
    int findImageIndexByPath(QString const& imagePath) const;

    /*!
     * @brief Build a collision-free destination path for a moved file.
     * @param initialTargetPath Preferred absolute destination path.
     * @return The preferred path when unused, otherwise a suffixed alternative
     *         such as `name (1).jpg`.
     */
    static QString createUniqueTargetPath(QString const& initialTargetPath);

    //! Clear the stored user-facing error state.
    void clearLastErrorMessage();

    /*!
     * @brief Store a user-facing error for the next UI query.
     * @param message Error text to expose through getLastErrorMessage().
     */
    void setLastErrorMessage(QString const& message);

    // [members]
    //! Current working directory containing the image session.
    QDir m_workingPath;

    //! Cached and sorted list of matching images in the working directory.
    QVector<QFileInfo> m_currentImages;

    //! Zero-based index into `m_currentImages`, or `-1` when no image is selected.
    int m_positionCurrentFile = -1;

    //! Undo/redo history for image move operations executed by this handler.
    CullendulaUndoStack m_undoStack;

    //! Active set of accepted image suffixes, normalized to lowercase.
    QSet<QString> m_allowedImageExtensions;

    //! Most recent user-facing error produced by a mutating operation.
    QString m_lastErrorMessage;
};

namespace CullendulaFileSystemHandlerDetail {
/*!
 * @brief Hooks used to test output-directory preparation failure paths.
 */
struct OutputFolderHooks {
    bool (*pathExists)(QString const& path);
    bool (*pathIsDirectory)(QString const& path);
    bool (*directoryExists)(QString const& path);
    bool (*mkdir)(QDir& parentDir, QString const& subdir);
};

/*!
 * @brief Build the default list of suggested image extensions from supported suffixes.
 * @param supportedSuffixes Lowercase Qt-supported image suffixes.
 * @return Up to ten normalized suffixes ordered by application preference.
 */
QStringList getSuggestedImageExtensions(QSet<QString> const& supportedSuffixes);

/*!
 * @brief Return the production hooks for output-directory handling.
 */
OutputFolderHooks defaultOutputFolderHooks();

/*!
 * @brief Prepare an application-managed subdirectory below a working path.
 * @param workingPath Parent directory used by the handler.
 * @param subdir Name of the application-managed subdirectory.
 * @param errorMessage Output parameter for a user-facing failure message.
 * @param hooks Filesystem operations to use.
 * @return `true` when the target directory exists after the call.
 */
bool createOutputFolder(QDir& workingPath, QString const& subdir, QString& errorMessage, OutputFolderHooks const& hooks);
}  // namespace CullendulaFileSystemHandlerDetail
