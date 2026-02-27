#include <QCoreApplication>
#include <QTest>

#include "Test_CullendulaUndoStack.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Test_CullendulaUndoStack test;
    return QTest::qExec(&test, argc, argv);
}
