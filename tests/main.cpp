#include <QtCore/QByteArray>
#include <QTest>
#include <QtWidgets/QApplication>

#include "Test_CullendulaFileSystemHandler.h"
#include "Test_CullendulaMainWindow.h"
#include "Test_CullendulaUndoStack.h"

int main(int argc, char *argv[])
{
    if(qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM"))
    {
        qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    }

    QApplication app(argc, argv);

    Test_CullendulaFileSystemHandler fileSystemTest;
    Test_CullendulaMainWindow mainWindowTest;
    Test_CullendulaUndoStack test;

    int status = 0;
    status |= QTest::qExec(&fileSystemTest, argc, argv);
    status |= QTest::qExec(&mainWindowTest, argc, argv);
    status |= QTest::qExec(&test, argc, argv);
    return status;
}
