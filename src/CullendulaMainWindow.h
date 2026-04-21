//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#pragma once

// own includes
#include "CullendulaFileSystemHandler.h"

// Qt includes
#include <QtCore/QMap>
#include <QtCore/QStringList>
#include <QtCore/QVector>
#include <QtGui/QPixmap>
#include <QtWidgets/QMainWindow>

namespace Ui {
class CullendulaMainWindow;
}

//----------------------------------------------------------------------------

/*!
 * @file
 * @brief Main application window for interactive image culling sessions.
 */

/*!
 * @brief Top-level Qt window that wires the UI to the file-system workflow.
 *
 * The main window owns the generated widget tree, reacts to drag-and-drop
 * events, delegates file operations to CullendulaFileSystemHandler, and keeps
 * menus, status feedback, and theme state synchronized with the active session.
 */
class CullendulaMainWindow : public QMainWindow {
    Q_OBJECT

   public:
    /*!
     * @brief Supported application palette variants.
     */
    enum class ThemeMode { Light, Dark };

    /*!
     * @brief Construct the main window and initialize actions, menus, and theme.
     * @param parent Optional Qt parent widget.
     */
    explicit CullendulaMainWindow(QWidget* parent = nullptr);

    //! Destroy the main window and its generated UI object.
    ~CullendulaMainWindow() override;

    /*!
     * @brief Return the currently active application theme.
     * @return The palette mode most recently applied through the window.
     */
    ThemeMode getThemeMode() const;

   protected:
    /*!
     * @brief Accept drag payloads so image folders or files can be dropped.
     * @param event Qt drag-enter event supplied by the window system.
     */
    void dragEnterEvent(QDragEnterEvent* event) override;

    /*!
     * @brief Load the first dropped file or directory into the current session.
     * @param event Qt drop event containing local URLs.
     */
    void dropEvent(QDropEvent* event) override;

    /*!
     * @brief Re-scale the displayed preview whenever the window size changes.
     * @param event Qt resize event with the new geometry.
     */
    void resizeEvent(QResizeEvent* event) override;

   private Q_SLOTS:
    //! Navigate to the previous image in the current directory session.
    void slotButtonLeftTriggered();

    //! Navigate to the next image in the current directory session.
    void slotButtonRightTriggered();

    //! Move the current image into the session's `output` directory.
    void slotButtonSaveTriggered();

    //! Move the current image into the session's `trash` directory.
    void slotButtonTrashTriggered();

    //! Show the application About dialog.
    void about();

    //! Show the Qt About dialog replacement used by this application.
    void aboutQt();

   private:
    /*!
     * @brief Show a themed, non-blocking information dialog.
     * @param title Dialog window title.
     * @param text Rich-text body shown inside the message box.
     */
    void showInformationDialog(QString const& title, QString const& text);

    /*!
     * @brief Push the checked extension menu entries into the file system handler.
     */
    void syncAllowedExtensionsToFileSystemHandler();

    /*!
     * @brief Apply one of the supported visual themes to the application.
     * @param themeMode Theme variant to activate.
     */
    void applyTheme(ThemeMode themeMode);

    /*!
     * @brief Refresh the central preview and the related window state.
     *
     * The method updates the image label, button enabled state, and status bar
     * message based on whether a current image is available.
     */
    void refreshLabel();

    /*!
     * @brief Enable or disable the navigation and move buttons as a group.
     * @param active When `true`, all action buttons become usable.
     */
    void activateButtons(bool const active = false) const;

    /*!
     * @brief Show a temporary message in the status bar.
     * @param message Text to display to the user.
     */
    void printStatus(QString const& message) const;

    //! Create QAction instances for menus, shortcuts, and toolbar-style commands.
    void createActions();

    //! Create the menu bar structure and insert the previously created actions.
    void createMenus();

    //! Synchronize the enabled state of undo and redo actions with the handler.
    void updateUndoRedoButtonStatus();

    /*!
     * @brief Load an image from disk into the in-memory preview cache when needed.
     * @param path Absolute image path to display.
     */
    void loadAndCachePhoto(QString const& path);

    //! Scale the cached image to the current label size and assign it to the UI.
    void showCachedPhoto();

    //! Clear the cached preview image and its source-path bookkeeping.
    void clearCachedPhoto();

    // [members]
    //! Generated widget tree from the `.ui` file.
    Ui::CullendulaMainWindow* ui;

    //! Backend object that manages session scanning, navigation, and file moves.
    CullendulaFileSystemHandler m_fileSystemHandler;

    //! Top-level menu that groups application configuration entries.
    QMenu* m_mainMenu = nullptr;

    //! Menu that exposes the selectable file-extension filters.
    QMenu* m_extensionsMenu = nullptr;

    //! Menu that lets the user switch between theme variants.
    QMenu* m_styleMenu = nullptr;

    //! One toggle action per supported image extension, keyed by lowercase suffix.
    QMap<QString, QAction*> m_extensionActions;

    //! Action that activates the light palette.
    QAction* m_lightThemeAction = nullptr;

    //! Action that activates the dark palette.
    QAction* m_darkThemeAction = nullptr;

    //! Edit menu that contains undo and redo commands.
    QMenu* m_editMenu = nullptr;

    //! QAction used to trigger undo of the last move.
    QAction* m_undoAction = nullptr;

    //! QAction used to trigger redo of the last undone move.
    QAction* m_redoQtAction = nullptr;

    //! Help menu containing application and Qt information dialogs.
    QMenu* m_helpMenu = nullptr;

    //! Action that opens the application About dialog.
    QAction* m_aboutAction = nullptr;

    //! Action that opens the Qt About dialog replacement.
    QAction* m_aboutQtAction = nullptr;

    //! Theme mode currently applied to the application palette and stylesheets.
    ThemeMode m_themeMode = ThemeMode::Light;

    //! Absolute path of the image currently stored in the preview cache.
    QString m_cachedImagePath;

    //! Original-resolution pixmap for the currently displayed image.
    QPixmap m_cachedPhoto;
};
