//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#include "CullendulaAppBootstrap.h"
#include "CullendulaMainWindow.h"

// Qt includes
#include <QtWidgets/QApplication>

//----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    CullendulaMainWindow mainWindow;
    CullendulaAppBootstrap::showMainWindow(mainWindow);

    return CullendulaAppBootstrap::runEventLoop(a);
}
