#ifndef CullendulaMainWindow_H
#define CullendulaMainWindow_H

#include <QMainWindow>

namespace Ui {
class CullendulaMainWindow;
}

class CullendulaMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CullendulaMainWindow(QWidget *parent = 0);
    ~CullendulaMainWindow();

private:
    Ui::CullendulaMainWindow *ui;
};

#endif // CullendulaMainWindow_H
