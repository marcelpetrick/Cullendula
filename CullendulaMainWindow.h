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
    //! v0.2 improved useability; more features (move to trash!); refactored code-base; improved code-quality
    QString const c_versionString = " - v0.2";
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
//    void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE; // todom check if necessary
//    void dragLeaveEvent(QDragLeaveEvent* event) Q_DECL_OVERRIDE; // todom check if necessary
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;

private slots:
    //! shift left and right (for- and backward) through the image-file-list (current directory)
    void slotButtonLeftTriggered();
    void slotButtonRightTriggered();
    //! save to the given output-folder (by moving)
    void slotButtonSaveTriggered();
    //! save to the given trash-folder (by moving)
    void slotButtonTrashTriggered();

private:
    //! Refresh the label: if file available, then show as image - else reset to the initial text
    void refreshLabel();

    //! Handle the state of the buttons.
    void activateButtons(bool const active = false);

// [members]
    // the ui
    Ui::CullendulaMainWindow* ui;

    // handles all the path- and file-related operations
    CullendulaFileSystemHandler m_fileSystemHandler;
};
