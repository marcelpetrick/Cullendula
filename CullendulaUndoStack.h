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
public: //[ctor]

    CullendulaUndoItem()
    {
        // nothing else to do :)
    }

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

class CullendulaUndoStack
{
public:
    CullendulaUndoStack();
    ~CullendulaUndoStack();

    // push: insert new pair (from, to); will be converted to CullendulaUndoItem
    void push(QString const& from, QString const& to);

    // will be replaced by "undo and redo" - was: pop: get last transaction
    //! TODO
    CullendulaUndoItem undo();
    CullendulaUndoItem redo();

    //! Return the current size; amount of items
    size_t getSize();

    // due to concerns while thinking about the API in combination with Redo, reading about the Command/Memento-Pattern is necessary ..
    // https://en.wikipedia.org/wiki/Command_pattern
    // https://en.wikipedia.org/wiki/Memento_pattern

private:
    //! container: keeps the data
    QVector<CullendulaUndoItem> m_container;

    //! Keeps track of the pointer. In case of undo it is quite simple just to pop the last item.
    //! Then redo would be enabled, because the pointer is set back by one.
    //! Redo walks back the pointer by one
    long m_currentItem; // initialized in ctor
};
