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
#include <QtWidgets/QMainWindow>

namespace Ui {
class CullendulaMainWindow;
}

//----------------------------------------------------------------------------

class CullendulaMainWindow : public QMainWindow {
    Q_OBJECT

   public:
    enum class ThemeMode { Light, Dark };

    explicit CullendulaMainWindow(QWidget* parent = nullptr);
    ~CullendulaMainWindow() override;

    ThemeMode getThemeMode() const;

   protected:
    //! Need to be reimplemented to make the drag&drop possible ... to get the path for the dropped file/folder
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

    //! Reimplemented to make the label with the current photo stretch accordingly.
    void resizeEvent(QResizeEvent* event) override;

   private Q_SLOTS:
    //! shift left and right (for- and backward) through the image-file-list (current directory)
    void slotButtonLeftTriggered();
    void slotButtonRightTriggered();
    //! save to the given output-folder (by moving)
    void slotButtonSaveTriggered();
    //! save to the given trash-folder (by moving)
    void slotButtonTrashTriggered();

    //! Slot for the help-menu-entries
    void about();

   private:
    //! Synchronize the selected extension menu entries into the file system handler.
    void syncAllowedExtensionsToFileSystemHandler();

    //! Apply one of the supported widget themes.
    void applyTheme(ThemeMode themeMode);

    //! Refresh the label: if file available, then show as image - else reset to the initial text
    void refreshLabel();

    //! Handle the state of the buttons.
    void activateButtons(bool const active = false) const;

    //! Print the given string to the status-bar.
    void printStatus(QString const& message) const;

    //! create actions and fitting menus for the "help-menu"
    void createActions();
    void createMenus();

    //! Called after each operation to display Undo/Redo with correct state.
    void updateUndoRedoButtonStatus();

    //! Load, scale and assign the current photo-file to the center-label.
    //! (Yes, could be resource-intensive when the user starts to play around with resizing.)
    void loadAndScalePhoto(const QString& path) const;

    // [members]
    //! the ui
    Ui::CullendulaMainWindow* ui;

    //! handles all the path- and file-related operations
    CullendulaFileSystemHandler m_fileSystemHandler;

    //! members for the "main"- and "edit"- menus
    QMenu* m_mainMenu = nullptr;
    QMenu* m_extensionsMenu = nullptr;
    QMenu* m_styleMenu = nullptr;
    QMap<QString, QAction*> m_extensionActions;
    QAction* m_lightThemeAction = nullptr;
    QAction* m_darkThemeAction = nullptr;
    QMenu* m_editMenu = nullptr;
    QAction* m_undoAction = nullptr;
    QAction* m_redoQtAction = nullptr;
    //! - end -

    //! members for the "help"-menu
    QMenu* m_helpMenu = nullptr;
    QAction* m_aboutAction = nullptr;
    QAction* m_aboutQtAction = nullptr;
    //! - end -

    ThemeMode m_themeMode = ThemeMode::Light;
};
