//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#pragma once

#include <QtCore/QTemporaryDir>
#include <QtTest/QtTest>
#include <memory>

#include "CullendulaMainWindow.h"

class QAction;
class QDialog;
class QLabel;
class QPushButton;
class QStatusBar;

/*!
 * @file
 * @brief QTest suite for the application's main window behavior.
 */

/*!
 * @brief Covers drag-and-drop loading, theme changes, menu wiring, and dialogs.
 */
class Test_CullendulaMainWindow : public QObject {
    Q_OBJECT

   private Q_SLOTS:
    //! Create a fresh temporary workspace and visible window before each test.
    void init();

    //! Destroy the window and temporary files after each test.
    void cleanup();

    //! Verify the window starts with the expected default widget state.
    void slot_Test_InitialState();

    //! Verify the light theme is active by default.
    void slot_Test_LightTheme_IsDefault();

    //! Verify the theme menu can switch the UI into dark mode.
    void slot_Test_ThemeMenu_SwitchesToDarkMode();

    //! Verify all suggested extensions start enabled.
    void slot_Test_ExtensionsMenu_DefaultsToAllChecked();

    //! Verify disabling every extension blocks the next load.
    void slot_Test_ExtensionsMenu_AllUncheckedBlocksNextDrop();

    //! Verify resizing without images keeps the fallback label text.
    void slot_Test_ResizeWithoutLoadedImages_ShowsFallbackText();

    //! Verify resizing a loaded image reuses the cached preview path.
    void slot_Test_ResizeLoadedImage_ReusesCachedPreview();

    //! Verify invalid image contents surface a preview fallback error.
    void slot_Test_InvalidImagePreview_ShowsFallbackError();

    //! Verify drag-enter accepts URL payloads before drop processing.
    void slot_Test_DragEnter_ValidUrlsAreAccepted();

    //! Verify drag-enter rejects unsupported payloads immediately.
    void slot_Test_DragEnter_InvalidPayloadIsRejected();

    //! Verify dropping a valid directory loads an image session.
    void slot_Test_DragEnterAndDropValidDirectory_LoadsImages();

    //! Verify extension menu changes affect the next dropped directory.
    void slot_Test_ExtensionsMenu_AffectsNextDroppedDirectory();

    //! Verify loading an empty directory clears the previous session state.
    void slot_Test_DroppingEmptyDirectory_ClearsPreviousSessionState();

    //! Verify unsupported drag payloads report an error status.
    void slot_Test_DropInvalidPayload_ShowsErrorStatus();

    //! Verify navigation and saving work through the main window controls.
    void slot_Test_ButtonNavigationAndSaveFlow();

    //! Verify trashing works through the main window controls.
    void slot_Test_ButtonTrashFlow();

    //! Verify the undo and redo actions move files on disk as expected.
    void slot_Test_UndoRedoActions_MoveFilesOnDisk();

    //! Verify failed save operations update the status bar with an error.
    void slot_Test_SaveFailure_ShowsStatusMessage();

    //! Verify the application About action opens a dialog and updates status.
    void slot_Test_AboutAction_ShowsDialogAndStatus();

    //! Verify the Qt About action opens a dialog and updates status.
    void slot_Test_AboutQtAction_ShowsDialogAndStatus();

   private:
    /*!
     * @brief Create a small test image below the temporary workspace.
     * @param relativePath Relative output path below the temporary directory.
     * @param color Fill color used for the generated image.
     * @return Absolute file path of the created image.
     */
    QString createImage(QString const& relativePath, QColor const& color = Qt::red);

    /*!
     * @brief Create a file with an image suffix but invalid image contents.
     * @param relativePath Relative output path below the temporary directory.
     * @return Absolute file path of the created file.
     */
    QString createInvalidImageFile(QString const& relativePath);

    /*!
     * @brief Simulate dropping one or more local URLs onto the window.
     * @param urls URLs to place into the synthetic drop event.
     */
    void sendDropWithUrls(QList<QUrl> const& urls);

    /*!
     * @brief Find a QAction by its visible text.
     * @param text Action text to search for.
     * @return Matching action or `nullptr`.
     */
    QAction* findAction(QString const& text) const;

    /*!
     * @brief Find an extension toggle action by suffix.
     * @param extension Lowercase file suffix without a leading dot.
     * @return Matching action or `nullptr`.
     */
    QAction* findExtensionAction(QString const& extension) const;

    /*!
     * @brief Find a theme action by its visible menu label.
     * @param themeName Visible action text to search for.
     * @return Matching action or `nullptr`.
     */
    QAction* findThemeAction(QString const& themeName) const;

    //! Find the currently open dialog owned by the window, if any.
    QDialog* findOpenDialog() const;

    /*!
     * @brief Find one of the main control buttons by object name.
     * @param name QObject name assigned in the UI file.
     * @return Matching push button or `nullptr`.
     */
    QPushButton* findButton(char const* name) const;

    //! Return the central preview label from the window UI.
    QLabel* findCenterLabel() const;

    //! Return the window status bar.
    QStatusBar* findStatusBar() const;

    //! Temporary root directory for filesystem-backed UI tests.
    std::unique_ptr<QTemporaryDir> m_tempDir;

    //! Main window instance under test.
    std::unique_ptr<CullendulaMainWindow> m_window;
};
