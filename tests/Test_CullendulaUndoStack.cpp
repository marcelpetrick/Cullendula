//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

// own includes
#include "Test_CullendulaUndoStack.h"

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::verifyUndoItem(CullendulaUndoItem const& item, QString const& expectedSource, QString const& expectedTarget) {
    QCOMPARE(item.sourcePath, expectedSource);
    QCOMPARE(item.targetPath, expectedTarget);
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::initTestCase() {
    // qDebug() << "Test_CullendulaUndoStack::initTestCase(): called before everything else";
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::cleanupTestCase() {
    // qDebug("Test_CullendulaUndoStack::cleanupTestCase(): called after myFirstTest and mySecondTest");
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::init() {
    // reset with a totally fresh item
    m_stackPtr = std::make_unique<CullendulaUndoStack>();
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::cleanup() {
    // reset with no new item
    m_stackPtr.reset();
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_Create_CullendulaUndoStack() {
    QVERIFY2(m_stackPtr->getUndoDepth() == 0, "CullendulaUndoStack was initialized and is empty");
    QVERIFY2(m_stackPtr->getRedoDepth() == 0, "CullendulaUndoStack was initialized and is empty");
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_Push() {
    QVERIFY(m_stackPtr->canUndo() == false);
    QVERIFY(m_stackPtr->canRedo() == false);

    m_stackPtr->push("a", "b");
    QCOMPARE(static_cast<int>(m_stackPtr->getUndoDepth()), 1);
    QVERIFY(m_stackPtr->canUndo() == true);
    QVERIFY(m_stackPtr->canRedo() == false);

    m_stackPtr->push("c", "d");
    QCOMPARE(static_cast<int>(m_stackPtr->getUndoDepth()), 2);
    QVERIFY(m_stackPtr->canUndo() == true);
    QVERIFY(m_stackPtr->canRedo() == false);
    m_stackPtr->undo();
    QVERIFY(m_stackPtr->canRedo() == true);
    m_stackPtr->undo();

    m_stackPtr->push("e", "f");
    QCOMPARE(static_cast<int>(m_stackPtr->getUndoDepth()), 1);
    QCOMPARE(static_cast<int>(m_stackPtr->getRedoDepth()), 0);
    QVERIFY(m_stackPtr->canUndo() == true);
    QVERIFY(m_stackPtr->canRedo() == false);
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_Undo() {
    // undo on an empty stack should not return something and not fail
    m_stackPtr->undo();
    m_stackPtr->undo();
    m_stackPtr->undo();

    CullendulaUndoItem const foo = m_stackPtr->undo();
    verifyUndoItem(foo, "", "");

    m_stackPtr->push("a", "b");

    CullendulaUndoItem const bar = m_stackPtr->undo();
    verifyUndoItem(bar, "a", "b");
    QCOMPARE(static_cast<int>(m_stackPtr->getUndoDepth()), 0);
    QCOMPARE(static_cast<int>(m_stackPtr->getRedoDepth()), 1);

    CullendulaUndoItem const item2 = m_stackPtr->undo();
    verifyUndoItem(item2, "", "");

    // test with 3 pushed items, then undo them in reverse order
    m_stackPtr->push("1", "2");
    m_stackPtr->push("3", "4");
    m_stackPtr->push("5", "6");
    QVERIFY(m_stackPtr->canUndo() == true);
    CullendulaUndoItem const item3 = m_stackPtr->undo();
    verifyUndoItem(item3, "5", "6");
    QVERIFY(m_stackPtr->canUndo() == true);
    CullendulaUndoItem const item4 = m_stackPtr->undo();
    verifyUndoItem(item4, "3", "4");
    QVERIFY(m_stackPtr->canUndo() == true);
    CullendulaUndoItem const item5 = m_stackPtr->undo();
    verifyUndoItem(item5, "1", "2");
    QVERIFY(m_stackPtr->canUndo() == false);  // should be false
    m_stackPtr->undo();
    QVERIFY(m_stackPtr->canUndo() == false);

    // pushing one element shall allow now some undo
    m_stackPtr->push("7", "8");
    QVERIFY(m_stackPtr->canUndo() == true);
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_Redo() {
    // redo on an empty stack should not return something and not fail
    m_stackPtr->redo();
    m_stackPtr->redo();
    m_stackPtr->redo();
    QVERIFY2(m_stackPtr->canRedo() == false, "can redo on empty stack: ERROR");

    // test that this returns an empty item
    CullendulaUndoItem const foo = m_stackPtr->redo();
    verifyUndoItem(foo, "", "");

    m_stackPtr->push("a", "b");

    m_stackPtr->undo();
    // now there should be one item on "redo"
    CullendulaUndoItem const bar = m_stackPtr->redo();
    verifyUndoItem(bar, "b", "a");
    QCOMPARE(static_cast<int>(m_stackPtr->getUndoDepth()), 1);
    QCOMPARE(static_cast<int>(m_stackPtr->getRedoDepth()), 0);

    CullendulaUndoItem const item2 = m_stackPtr->redo();
    verifyUndoItem(item2, "", "");
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_UndoRedoLoop() {
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
    verifyUndoItem(foo0, "5", "6");

    // redo should be possible
    QVERIFY(m_stackPtr->canRedo() == true);

    // redo should yield the inverse move, because redo replays the move action.
    CullendulaUndoItem const foo1 = m_stackPtr->redo();
    verifyUndoItem(foo1, "6", "5");

    // The original move is back on the undo stack.
    CullendulaUndoItem const foo2 = m_stackPtr->undo();
    verifyUndoItem(foo2, "5", "6");

    // undo again
    m_stackPtr->undo();

    // Redo should replay the move for the second item.
    CullendulaUndoItem const foo3 = m_stackPtr->redo();
    verifyUndoItem(foo3, "4", "3");

    // undo again
    m_stackPtr->undo();

    // undo: is now (1,2)
    CullendulaUndoItem const foo4 = m_stackPtr->undo();
    verifyUndoItem(foo4, "1", "2");

    // check if canUndo == false
    QVERIFY(m_stackPtr->canUndo() == false);
    // check if redo would be possible
    QVERIFY(m_stackPtr->canRedo() == true);
    // check if redo would be possible three times
    QCOMPARE(static_cast<int>(m_stackPtr->getRedoDepth()), 3);
}

//----------------------------------------------------------------------------------

// uncomment the following line to make the unit-test runnable
//! @attention Moved to main.cpp
// QTEST_MAIN(Test_CullendulaUndoStack)
