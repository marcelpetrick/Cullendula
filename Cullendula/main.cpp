#include "CullendulaMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CullendulaMainWindow w;
    w.show();

    return a.exec();
}
