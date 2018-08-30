//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#include "Test_CullendulaUndoStack.h"

//----------------------------------------------------------------------------------

//Test_CullendulaUndoStack::Test_CullendulaUndoStack()
//{

//}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::initTestCase()
{
    qDebug() << "Test_CullendulaUndoStack::initTestCase(): called before everything else";
    m_stackPtr = std::make_shared<CullendulaUndoStack>();
}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::cleanupTestCase()
{
    qDebug("Test_CullendulaUndoStack::cleanupTestCase(): called after myFirstTest and mySecondTest");
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
    m_stackPtr->push("a", "b");
    QVERIFY2(m_stackPtr->getSize() == 2, "after pushing another one");
}

//----------------------------------------------------------------------------------

// uncomment the following line to make the unit-test runnable
QTEST_MAIN(Test_CullendulaUndoStack)

