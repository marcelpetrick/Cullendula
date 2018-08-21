//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#include "CullendulaMainWindow.h"

//Qt includes
#include <QtWidgets/QApplication>

//----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    CullendulaMainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}
