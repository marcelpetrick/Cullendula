// Qt includes
#include <QtTest/QtTest>
#include <QtCore/QDebug>

#pragma once

// to read: http://doc.qt.io/qt-5/qtest-overview.html
//initTestCase() will be called before the first test function is executed.
//cleanupTestCase() will be called after the last test function was executed.
//init() will be called before each test function is executed.
//cleanup() will be called after every test function.

class Test_CullendulaUndoStack: public QObject
{
    Q_OBJECT

public:
    //    Test_CullendulaUndoStack();

private slots:
    void initTestCase()
        { qDebug("Test_CullendulaUndoStack: called before everything else"); }

    void cleanupTestCase()
        { qDebug("Test_CullendulaUndoStack: called after myFirstTest and mySecondTest"); }

    void slot_Test_Creat_CullendulaUndoStack();

    void slot_Test_Push();
};
