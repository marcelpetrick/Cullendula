//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

// own includes
#include "Test_CullendulaUndoStack.h"

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::initTestCase()
{
    qDebug() << "Test_CullendulaUndoStack::initTestCase(): called before everything else";
    //m_stackPtr = std::make_shared<CullendulaUndoStack>(); // will be done now via init()
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::cleanupTestCase()
{
    qDebug("Test_CullendulaUndoStack::cleanupTestCase(): called after myFirstTest and mySecondTest");
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::init()
{
    // reset with a new item
    m_stackPtr.reset(new CullendulaUndoStack);
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::cleanup()
{
    // reset with no new item:
    m_stackPtr.reset();
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_Create_CullendulaUndoStack()
{
    QVERIFY2(m_stackPtr->getSize() == 0, "CullendulaUndoStack was initialized and is empty");
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_Push()
{
    m_stackPtr->push("a", "b");
    QVERIFY2(m_stackPtr->getSize() == 1, "after pushing one");
    m_stackPtr->push("c", "d");
    QVERIFY2(m_stackPtr->getSize() == 2, "after pushing another one");
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_Pop()
{
    CullendulaUndoItem const foo = m_stackPtr->undo();
    // should yield ("","")
    bool const isEmptyItem = foo.fromPath.isEmpty() && foo.toPath.isEmpty();
    QVERIFY2(isEmptyItem == true, "undo on empty stack");

    m_stackPtr->push("a", "b");

    CullendulaUndoItem const bar = m_stackPtr->undo();
    // should yield ("a","b")
    bool const isEmptyItem2 = !bar.fromPath.isEmpty() && !bar.toPath.isEmpty();
    QVERIFY2(isEmptyItem2 == true, "undo on 1 item-stack");

    //! @todo big mistake! something is wrong here: first undo yields ("","")
}

//----------------------------------------------------------------------------------

// uncomment the following line to make the unit-test runnable
QTEST_MAIN(Test_CullendulaUndoStack)

