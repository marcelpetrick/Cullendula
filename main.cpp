#include "CullendulaMainWindow.h"

//Qt includes
#include <QtWidgets/QApplication>

//----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CullendulaMainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
