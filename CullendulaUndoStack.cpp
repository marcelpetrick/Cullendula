//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

// own includes
#include "CullendulaUndoStack.h"

// Qt includes
#include <QtCore/QDebug> //todom maybe remove

//----------------------------------------------------------------------------------

CullendulaUndoStack::CullendulaUndoStack()
{
    qDebug() << "CullendulaUndoStack::CullendulaUndoStack()";
    m_container.reserve(10); // TODO

    //! @todo
}

//----------------------------------------------------------------------------------

void CullendulaUndoStack::push(const QString &from, const QString &to)
{
    //! @todo
}

//----------------------------------------------------------------------------------

CullendulaUndoItem CullendulaUndoStack::undo()
{
    //! @todo
    return CullendulaUndoItem();
}

//----------------------------------------------------------------------------------

CullendulaUndoItem CullendulaUndoStack::redo()
{
    //! @todo
    return CullendulaUndoItem();
}

//----------------------------------------------------------------------------------

size_t CullendulaUndoStack::getSize()
{
    return static_cast<size_t>(m_container.size());
}

//----------------------------------------------------------------------------------
