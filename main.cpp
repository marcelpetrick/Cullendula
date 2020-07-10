//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

// uncomment the following line for the "run-able" program Cullendula. Else the test-suite is executed.
//#define runTests

//----------------------------------------------------------------------------

#include "CullendulaMainWindow.h"
#if defined(runTests)
#include "Test_CullendulaUndoStack.h"
#endif

//Qt includes
#include <QtWidgets/QApplication>

//----------------------------------------------------------------------------

#if defined(runTests)
    QTEST_MAIN(Test_CullendulaUndoStack)
#else
    int main(int argc, char *argv[])
    {
        QApplication a(argc, argv);
        CullendulaMainWindow mainWindow;
        mainWindow.show();

        return QApplication::exec();
    }
#endif
