//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#pragma once

// Qt includes
#include <QtCore/QVector>
#include <QtCore/QString>

class CullendulaUndoStack
{
public:
    CullendulaUndoStack();

    // push: insert new pair (from, to)

    // pop: get last transaction

    // get the current size; amount of items

    // due to concerns while thinking about the API in combination with Redo, reading about the Command/Memento-Pattern is necessary ..
    // https://en.wikipedia.org/wiki/Command_pattern
    // https://en.wikipedia.org/wiki/Memento_pattern

private:
    //! container
    QVector<QString> m_container;
};
