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
    //qDebug() << "Test_CullendulaUndoStack::initTestCase(): called before everything else";
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::cleanupTestCase()
{
    //qDebug("Test_CullendulaUndoStack::cleanupTestCase(): called after myFirstTest and mySecondTest");
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::init()
{
    // reset with a totally fresh item
    m_stackPtr.reset(new CullendulaUndoStack);
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::cleanup()
{
    // reset with no new item
    m_stackPtr.reset();
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_Create_CullendulaUndoStack()
{
    QVERIFY2(m_stackPtr->getUndoDepth() == 0, "CullendulaUndoStack was initialized and is empty");
    QVERIFY2(m_stackPtr->getRedoDepth() == 0, "CullendulaUndoStack was initialized and is empty");
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_Push()
{
    QVERIFY(m_stackPtr->canUndo() == false);
    QVERIFY(m_stackPtr->canRedo() == false);
    m_stackPtr->push("a", "b");
    QVERIFY2(m_stackPtr->getUndoDepth() == 1, "after pushing one");
    QVERIFY(m_stackPtr->canUndo() == true);
    QVERIFY(m_stackPtr->canRedo() == false);
    m_stackPtr->push("c", "d");
    QVERIFY2(m_stackPtr->getUndoDepth() == 2, "after pushing another one");
    QVERIFY(m_stackPtr->canUndo() == true);
    QVERIFY(m_stackPtr->canRedo() == false);
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_Undo()
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
    QVERIFY2((item2.fromPath == "") && (item2.toPath == ""), "undo on empty item-stack");

    // test with 3 pushed items, then undo them in reverse order
    m_stackPtr->push("1", "2");
    m_stackPtr->push("3", "4");
    m_stackPtr->push("5", "6");
    QVERIFY(m_stackPtr->canUndo() == true);
    CullendulaUndoItem const item3 = m_stackPtr->undo();
    QVERIFY2((item3.fromPath == "5") && (item3.toPath == "6"), "undo on 1 item-stack");
    QVERIFY(m_stackPtr->canUndo() == true);
    CullendulaUndoItem const item4 = m_stackPtr->undo();
    QVERIFY2((item4.fromPath == "3") && (item4.toPath == "4"), "undo on 1 item-stack");
    QVERIFY(m_stackPtr->canUndo() == true);
    CullendulaUndoItem const item5 = m_stackPtr->undo();
    QVERIFY2((item5.fromPath == "1") && (item5.toPath == "2"), "undo on 1 item-stack");
    QVERIFY(m_stackPtr->canUndo() == false); // should be false
    m_stackPtr->undo();
    QVERIFY(m_stackPtr->canUndo() == false);

    //pushing one element shall allow now some undo
    m_stackPtr->push("7", "8");
    QVERIFY(m_stackPtr->canUndo() == true);
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_Redo()
{
    // redo on an empty stack should not return something and not fail
    m_stackPtr->redo();
    m_stackPtr->redo();
    m_stackPtr->redo();
    QVERIFY2(m_stackPtr->canRedo() == false, "can redo on empty stack: ERROR");

    // test that this returns an empty item
    CullendulaUndoItem const foo = m_stackPtr->redo();
    // should yield ("","")
    bool const isEmptyItem = foo.fromPath.isEmpty() && foo.toPath.isEmpty();
    QVERIFY2(isEmptyItem, "redo on empty stack");

    m_stackPtr->push("a", "b");

    m_stackPtr->undo();
    // now there should be one item on "redo"
    CullendulaUndoItem const bar = m_stackPtr->redo();
    // should yield ("a","b")
    bool const hasExpectedContent = (bar.fromPath == "a") && (bar.toPath == "b");
    QVERIFY2(hasExpectedContent, "redo on 1 item-stack");

    CullendulaUndoItem const item2 = m_stackPtr->redo();
    QVERIFY2((item2.fromPath == "") && (item2.toPath == ""), "undo on empty item-stack");
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_UndoRedoLoop()
{
    // testplan:
    // push three items
    m_stackPtr->push("1", "2");
    m_stackPtr->push("3", "4");
    m_stackPtr->push("5", "6");

    // check if undo would be possible three times
    QCOMPARE(static_cast<int>(m_stackPtr->getUndoDepth()), 3);

    // redo should be false
    QVERIFY(m_stackPtr->canRedo() == false);

    // undo should yield last item (5,6)
    CullendulaUndoItem const foo0 = m_stackPtr->undo();
    bool const hasExpectedContent0 = (foo0.fromPath == "5") && (foo0.toPath == "6");
    QVERIFY(hasExpectedContent0);

    // redo should be possible
    QVERIFY(m_stackPtr->canRedo() == true);

    // redo should yield (5,6)
    CullendulaUndoItem const foo1 = m_stackPtr->redo();
    bool const hasExpectedContent1 = (foo1.fromPath == "5") && (foo1.toPath == "6");
    QVERIFY(hasExpectedContent1);

    // --> this means that (5,6) is at undo? check this
    CullendulaUndoItem const foo2 = m_stackPtr->undo();
    bool const hasExpectedContent2 = (foo2.fromPath == "5") && (foo2.toPath == "6");
    QVERIFY(hasExpectedContent2);

    // undo again
    m_stackPtr->undo();

    // check redo for (3,4)
    CullendulaUndoItem const foo3 = m_stackPtr->redo();
    bool const hasExpectedContent3 = (foo3.fromPath == "3") && (foo3.toPath == "4");
    QVERIFY(hasExpectedContent3);

    // undo again
    m_stackPtr->undo();

    // undo: is now (1,2)
    CullendulaUndoItem const foo4 = m_stackPtr->undo();
    bool const hasExpectedContent4 = (foo4.fromPath == "1") && (foo4.toPath == "2");
    QVERIFY(hasExpectedContent4);

    // check if canUndo == false
    QVERIFY(m_stackPtr->canUndo() == false);
    // check if redo would be possible
    QVERIFY(m_stackPtr->canRedo() == true);
    // check if redo would be possible three times
    QCOMPARE(static_cast<int>(m_stackPtr->getRedoDepth()), 3);
}

//----------------------------------------------------------------------------------

// uncomment the following line to make the unit-test runnable
QTEST_MAIN(Test_CullendulaUndoStack)
