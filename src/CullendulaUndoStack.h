//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

// due to concerns while thinking about the API in combination with Redo, reading about the Command/Memento-Pattern is necessary ..
// https://en.wikipedia.org/wiki/Command_pattern
// https://en.wikipedia.org/wiki/Memento_pattern

#pragma once

// Qt includes
#include <QtCore/QVector>
#include <QtCore/QString>

//----------------------------------------------------------------------------------

//! @class CullendulaUndoItem Helper to store the "source, target"-tuples for the moved files.
class CullendulaUndoItem
{
public:
    //[ctor]
    CullendulaUndoItem() = default;
    //[dtor]
    ~CullendulaUndoItem() = default;
    //[ctor] - just init from the given value
    CullendulaUndoItem(QString const& from, QString const& to) :
        sourcePath(from),
        targetPath(to)
    {
        // nothing else to do :)
    }

    //[members]

    //! Quite simple: container for a pair of two paths (saved as String).
    //! Right now access is unlimited.
    QString sourcePath;
    QString targetPath;
};

//----------------------------------------------------------------------------------

//! @class CullendulaUndoStack Helper to encapsulate some undo- and redo-functionality.
//!         Available operations shall be "undo" and "redo" and "push".
//!         "canUndo" and "canRedo" tell if the specific operations are available.
//!         In case of executing undo/redo when the respective checks would fail,
//!         an empty, fresh item is returned.
//! @todo   The big and open question is: should undo flip the items while putting them to redo?
//!         And redo vice versa? Because this affects how the usage has to be implemented.
//!         For now: no flipping. (a,b) stays (a,b) at redo.
//!
class CullendulaUndoStack
{
public:
    CullendulaUndoStack() = default;
    ~CullendulaUndoStack() = default;

    //! Push: insert new pair (from, to); will be converted to CullendulaUndoItem.
    //! Will fill the undo-stack (first).
    void push(QString const& from, QString const& to);

    //! Return the last added item from the undo-"stack", if possible.
    //! Else return: new default item.
    CullendulaUndoItem undo();
    //! Return the last added item from the redo-"stack", if possible.
    //! Else return: new default item.
    CullendulaUndoItem redo();

    //! Checks if the option is possible. Useful for the GUI (dis-/enabled).
    //! The undo-container has items.
    bool canUndo();
    //! The redo-container has items.
    bool canRedo();

    //! Return the current size (= amount of items). Useful for the unit-test.
    long getUndoDepth();
    long getRedoDepth();

private:
    //! container: keeps the data for Undo
    QVector<CullendulaUndoItem> m_undoContainer;

    //! container: keeps the data for Redo
    QVector<CullendulaUndoItem> m_redoContainer;
};
