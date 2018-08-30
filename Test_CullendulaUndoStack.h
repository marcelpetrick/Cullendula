//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

// own includes
#include <CullendulaUndoStack.h>

// Qt includes
#include <QtTest/QtTest>
#include <QtCore/QDebug>

// std-includes
#include <memory> // for unique_ptr

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
    // not needed right now
    //    Test_CullendulaUndoStack();

private slots:
    //! -- slots from QTEST --

    //initTestCase() will be called before the first test function is executed.
    void initTestCase();

    //cleanupTestCase() will be called after the last test function was executed.
    void cleanupTestCase();

    //init() will be called before each test function is executed.
    void init();

    //cleanup() will be called after every test function.
    void cleanup();

    //! -- end of: slots from QTEST --

    void slot_Test_Create_CullendulaUndoStack();

    void slot_Test_Push();

    //! Used to test if the returned result after popping is the same like expected
    //! Pop == Undo
    void slot_Test_Pop();

private:
    //! contains the used undo-stack
    std::shared_ptr<CullendulaUndoStack> m_stackPtr = nullptr;
};
