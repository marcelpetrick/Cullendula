//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#pragma once

// own includes
#include "CullendulaFileSystemHandler.h"

// Qt includes
#include <QtWidgets/QMainWindow>

namespace Ui {
    class CullendulaMainWindow;
}

// for constants
namespace {
    //! v0.1 was the basic release; working, but ugly
    //! v0.2 improved useability and stability; more features (move to trash!); refactored code-base; improved code-quality
    //! v0.3 added tooltips; fixed the "pumping center-label"-issue; added menus; fixed some resizing-issues with the image-label
    QString const c_versionString = " - v0.3.2";
    unsigned int const c_StatusBarDelay = 5000;
    int const c_extraWidthBecauseOfFraming = 2;
}

class CullendulaMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CullendulaMainWindow(QWidget* parent = nullptr);
    ~CullendulaMainWindow() override;

protected:
    //! Need to be reimplemented to make the drag&drop possible ... to get the path for the dropped file/folder
    void dragEnterEvent(QDragEnterEvent* event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;

    //! Reimplemented to make the label with the current photo stretch accordingly.
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    //! shift left and right (for- and backward) through the image-file-list (current directory)
    void slotButtonLeftTriggered();
    void slotButtonRightTriggered();
    //! save to the given output-folder (by moving)
    void slotButtonSaveTriggered();
    //! save to the given trash-folder (by moving)
    void slotButtonTrashTriggered();

    //! Slots for the help-menu-entries
    void about();
    void aboutQt();
    //! - end -

private:
    //! Refresh the label: if file available, then show as image - else reset to the initial text
    void refreshLabel();

    //! Handle the state of the buttons.
    void activateButtons(bool const active = false);

    //! Print the given string to the status-bar.
    void printStatus(QString const & message);

    //! create actions and fitting menues for the "help-menu"
    void createActions();
    void createMenus();

    //! Load, scale and assigne the current photo-file to the center-label.
    //! (Yes, could be ressource-intensive when the user starts to play around with resizing.)
    void loadAndScalePhoto(const QString &path);

// [members]
    //! the ui
    Ui::CullendulaMainWindow* ui;

    //! handles all the path- and file-related operations
    CullendulaFileSystemHandler m_fileSystemHandler;

    //! members for the help menu
    QMenu* helpMenu = nullptr;
    QAction* aboutAct = nullptr;
    QAction* aboutQtAct = nullptr;
    //! - end -
};
