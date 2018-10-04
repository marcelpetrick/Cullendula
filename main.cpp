//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#include "CullendulaMainWindow.h"
#include "Test_CullendulaUndoStack.h"

//Qt includes
#include <QtWidgets/QApplication>

//----------------------------------------------------------------------------

// uncomment the following line for the "run-able" program Cullendula. Else the test-suite is executed.
//#define runtests

#if defined(runtests)
    QTEST_MAIN(Test_CullendulaUndoStack)
#else
    int main(int argc, char *argv[])
    {
        QApplication a(argc, argv);
        CullendulaMainWindow mainWindow;
        mainWindow.show();

        return a.exec();
    }
#endif
