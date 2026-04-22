//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#pragma once

// own includes
#include "CullendulaUndoStack.h"

// Qt includes
#include <QtTest/QtTest>

// std includes
#include <memory>

/*!
 * @file
 * @brief QTest suite for the undo/redo stack primitives.
 */

/*!
 * @brief Validates history push, undo, redo, and state bookkeeping.
 */
class Test_CullendulaUndoStack : public QObject {
    Q_OBJECT

   private Q_SLOTS:
    //! Allocate long-lived test fixtures before the first test runs.
    static void initTestCase();

    //! Release long-lived test fixtures after the final test.
    static void cleanupTestCase();

    //! Reset state before each individual test.
    void init();

    //! Tear down per-test state after each test.
    void cleanup();

    //! Verify default construction of the undo stack.
    void slot_Test_Create_CullendulaUndoStack();

    //! Verify that push adds items to the undo history.
    void slot_Test_Push();

    //! Verify that undo returns the expected item and updates stack state.
    void slot_Test_Undo();

    //! Verify redo after a preceding undo.
    void slot_Test_Redo();

    //! Verify alternating undo and redo operations over multiple items.
    void slot_Test_UndoRedoLoop();

    //! Verify undo on an empty stack leaves all state untouched.
    void slot_Test_Undo_OnEmptyStack_PreservesEmptyState();

    //! Verify redo on an empty stack leaves all state untouched.
    void slot_Test_Redo_OnEmptyStack_PreservesEmptyState();

    //! Verify a fresh push clears redo history after partial undo.
    void slot_Test_Push_AfterUndo_ClearsRedoHistory();

   private:
    /*!
     * @brief Compare an undo item against its expected source and target paths.
     * @param item Item returned by the stack.
     * @param expectedSource Expected source path.
     * @param expectedTarget Expected target path.
     */
    static void verifyUndoItem(CullendulaUndoItem const& item, QString const& expectedSource, QString const& expectedTarget);

    //! Stack instance under test.
    std::unique_ptr<CullendulaUndoStack> m_stackPtr = nullptr;
};
