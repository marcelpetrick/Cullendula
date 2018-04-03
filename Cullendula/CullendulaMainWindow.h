#pragma once

#include <QtWidgets/QMainWindow>

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
    // make the drag&drop possible ... to get the path for the dropped file/folder
    void dragEnterEvent(QDragEnterEvent* event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent* event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;

private:
    // the ui
    Ui::CullendulaMainWindow* ui;
};
