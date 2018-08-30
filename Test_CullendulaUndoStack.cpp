#include "Test_CullendulaUndoStack.h"

//----------------------------------------------------------------------------------

//Test_CullendulaUndoStack::Test_CullendulaUndoStack()
//{

//}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_Creat_CullendulaUndoStack()
{
    //! @todo code was taken over from example ..
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));

    // TODO implement the creation of the real test

}

//----------------------------------------------------------------------------------

void Test_CullendulaUndoStack::slot_Test_Push()
{
    QCOMPARE(0,1);
}

//----------------------------------------------------------------------------------

// uncomment the following line to make the unit-test runnable
QTEST_MAIN(Test_CullendulaUndoStack)

