#include <QCoreApplication>
#include <QTest>

#include "Test_CullendulaUndoStack.h"

int main(int argc, char *argv[])
{
    Test_CullendulaUndoStack test;

    QTest::qExec(&test);
}
