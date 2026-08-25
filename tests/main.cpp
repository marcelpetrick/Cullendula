#include <QTest>
#include <QtCore/QByteArray>
#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtWidgets/QApplication>

#include "CullendulaAppBootstrap.h"
#include "Test_CullendulaAppBootstrap.h"
#include "Test_CullendulaFileSystemHandler.h"
#include "Test_CullendulaMainWindow.h"
#include "Test_CullendulaUndoStack.h"

namespace {
/*!
 * @brief Run one test object, optionally writing its report to its own file.
 *
 * All test objects share one process, and QTest truncates the file given with -o, so a
 * single output file would only ever keep the report of the object that ran last. When
 * CULLENDULA_TEST_REPORT_DIR is set, each object therefore writes its own report. This
 * matters on platforms where the test output does not reach the console at all, which is
 * the case for this executable on Windows.
 *
 * @param testObject Test object to execute.
 * @param name Base name for the report file of this object.
 * @param arguments Command line arguments to forward to QTest.
 * @return Number of failed test functions reported by QTest.
 */
int runTestObject(QObject* testObject, QString const& name, QStringList const& arguments) {
    QString const reportDirectory = qEnvironmentVariable("CULLENDULA_TEST_REPORT_DIR");
    if (reportDirectory.isEmpty()) {
        return QTest::qExec(testObject, arguments);
    }

    QStringList argumentsWithReport = arguments;
    argumentsWithReport << QStringLiteral("-o") << QDir(reportDirectory).filePath(name + ".txt") + QStringLiteral(",txt");

    return QTest::qExec(testObject, argumentsWithReport);
}
}  // namespace

int main(int argc, char* argv[]) {
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")) {
        CullendulaAppBootstrap::ensureQtPlatformPluginForTests();
    }

    QApplication app(argc, argv);

    QStringList arguments;
    arguments.reserve(argc);
    for (int index = 0; index < argc; ++index) {
        arguments << QString::fromLocal8Bit(argv[index]);
    }

    Test_CullendulaAppBootstrap appBootstrapTest;
    Test_CullendulaFileSystemHandler fileSystemTest;
    Test_CullendulaMainWindow mainWindowTest;
    Test_CullendulaUndoStack undoStackTest;

    int status = 0;
    status |= runTestObject(&appBootstrapTest, QStringLiteral("Test_CullendulaAppBootstrap"), arguments);
    status |= runTestObject(&fileSystemTest, QStringLiteral("Test_CullendulaFileSystemHandler"), arguments);
    status |= runTestObject(&mainWindowTest, QStringLiteral("Test_CullendulaMainWindow"), arguments);
    status |= runTestObject(&undoStackTest, QStringLiteral("Test_CullendulaUndoStack"), arguments);
    return status;
}
