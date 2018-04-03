#pragma once

// Qt includes
#include <QtWidgets/QMainWindow>
#include <QtCore/QDir>

namespace Ui {
class CullendulaMainWindow;
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
    void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE; // todom check if necessary
    void dragLeaveEvent(QDragLeaveEvent* event) Q_DECL_OVERRIDE; // todom check if necessary
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;

private:
    // [methods]
    //! checks the currently set path and gets the file-list. Afterwards triggers loading of the current image.
    void processNewPath();

    //! Scan given path for image-file-types (for now suffix: jpg, jpeg - more later)
    void createImageFileList();

    //! Create and check if an output-folder exists.
    //! Initially: just create sub-dir "output".
    void createOutputFolder();

    // [members]
    // the ui
    Ui::CullendulaMainWindow* ui;

    // the currently chosen path to the directory (not file)
    QDir m_workingPath;
};
