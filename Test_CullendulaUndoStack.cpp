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
    QVERIFY(m_stackPtr->canUndo() == false);
    QVERIFY(m_stackPtr->canRedo() == false);
    m_stackPtr->push("a", "b");
    QVERIFY2(m_stackPtr->getSize() == 1, "after pushing one");
    QVERIFY(m_stackPtr->canUndo() == true); // fails currently
    QVERIFY(m_stackPtr->canRedo() == false);
    m_stackPtr->push("c", "d");
    QVERIFY2(m_stackPtr->getSize() == 2, "after pushing another one");
    QVERIFY(m_stackPtr->canUndo() == true);
    QVERIFY(m_stackPtr->canRedo() == false);
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_Pop()
{
    // undo on an empty stack should not return something and not fail
    m_stackPtr->undo();
    m_stackPtr->undo();
    m_stackPtr->undo();

    CullendulaUndoItem const foo = m_stackPtr->undo();
    // should yield ("","")
    bool const isEmptyItem = foo.fromPath.isEmpty() && foo.toPath.isEmpty();
    QVERIFY2(isEmptyItem, "undo on empty stack");

    m_stackPtr->push("a", "b");

    CullendulaUndoItem const bar = m_stackPtr->undo();
    // should yield ("a","b")
    bool const hasExpectedContent = (bar.fromPath == "a") && (bar.toPath == "b");
    QVERIFY2(hasExpectedContent, "undo on 1 item-stack");

    CullendulaUndoItem const item2 = m_stackPtr->undo();
    QVERIFY2((item2.fromPath == "") && (item2.toPath == ""), "undo on 1 item-stack");

    // test with 3 pushed items, then undo them in reverse order
    m_stackPtr->push("1", "2");
    m_stackPtr->push("3", "4");
    m_stackPtr->push("5", "6");
    CullendulaUndoItem const item3 = m_stackPtr->undo();
    QVERIFY2((item3.fromPath == "5") && (item3.toPath == "6"), "undo on 1 item-stack");
    CullendulaUndoItem const item4 = m_stackPtr->undo();
    QVERIFY2((item4.fromPath == "3") && (item4.toPath == "4"), "undo on 1 item-stack");
    CullendulaUndoItem const item5 = m_stackPtr->undo();
    QVERIFY2((item5.fromPath == "1") && (item5.toPath == "2"), "undo on 1 item-stack");
}

//----------------------------------------------------------------------------------

// uncomment the following line to make the unit-test runnable
QTEST_MAIN(Test_CullendulaUndoStack)

