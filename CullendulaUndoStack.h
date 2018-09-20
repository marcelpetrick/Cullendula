//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#pragma once

// Qt includes
#include <QtCore/QVector>
#include <QtCore/QString>

//----------------------------------------------------------------------------------

//! @class Helper to store the "source, target"-tuples for the moved files.
class CullendulaUndoItem
{
public:
    //[ctor]
    CullendulaUndoItem() = default;
    //[dtor]
    ~CullendulaUndoItem() = default;
    //[ctor] - just init from the given value
    CullendulaUndoItem(QString const& from, QString const& to) :
        fromPath(from),
        toPath(to)
    {
        // nothing else to do :)
    }

public: //[members]
    //! Quite simple: container for a pair of two paths (saved as String).
    //! Right now access is unlimited.
    QString fromPath;
    QString toPath;
};

//----------------------------------------------------------------------------------

//! @class CullendulaUndoStack - helper to encapsulate some undo- and redo-functionality.
//!         Available operations shall be "undo" and "redo" and "push".
//!         "canUndo" and "canRedo" tell if the specific operations are available.
//!         Undo only when at least a non-empty item can be returned.
//!         TODO: fix this.
//!
//! @attention Redo currently not supported.
class CullendulaUndoStack
{
public:
    CullendulaUndoStack();
    ~CullendulaUndoStack();

    //! Push: insert new pair (from, to); will be converted to CullendulaUndoItem.
    //! Will fill the undo-stack (first).
    void push(QString const& from, QString const& to);

    // will be replaced by "undo and redo" - was: pop: get last transaction
    //! TODO
    CullendulaUndoItem undo();
    CullendulaUndoItem redo();

    //! Checks if the option is possible. Useful for the GUI (dis-/enabled).
    bool canUndo();
    bool canRedo();

    //! Return the current size (= amount of items). Useful for the unit-test.
    long getSize();

    // due to concerns while thinking about the API in combination with Redo, reading about the Command/Memento-Pattern is necessary ..
    // https://en.wikipedia.org/wiki/Command_pattern
    // https://en.wikipedia.org/wiki/Memento_pattern

private:
    //! container: keeps the data for Undo
    QVector<CullendulaUndoItem> m_undoContainer;

    //! container: keeps the data for Redo
    QVector<CullendulaUndoItem> m_redoContainer;
};
