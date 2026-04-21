#include <QTest>
#include <QtCore/QByteArray>
#include <QtWidgets/QApplication>

#include "CullendulaAppBootstrap.h"
#include "Test_CullendulaAppBootstrap.h"
#include "Test_CullendulaFileSystemHandler.h"
#include "Test_CullendulaMainWindow.h"
#include "Test_CullendulaUndoStack.h"

int main(int argc, char* argv[]) {
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")) {
        CullendulaAppBootstrap::ensureQtPlatformPluginForTests();
    }

    QApplication app(argc, argv);

    Test_CullendulaAppBootstrap appBootstrapTest;
    Test_CullendulaFileSystemHandler fileSystemTest;
    Test_CullendulaMainWindow mainWindowTest;
    Test_CullendulaUndoStack test;

    int status = 0;
    status |= QTest::qExec(&appBootstrapTest, argc, argv);
    status |= QTest::qExec(&fileSystemTest, argc, argv);
    status |= QTest::qExec(&mainWindowTest, argc, argv);
    status |= QTest::qExec(&test, argc, argv);
    return status;
}
