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
    qDebug() << "CullendulaUndoStack::CullendulaUndoStack(): ctor";
    //m_container.reserve(10); // TODO

    // update and initialize the current position
    m_currentItem = static_cast<long>(getSize()) - 1;
}

//----------------------------------------------------------------------------------

CullendulaUndoStack::~CullendulaUndoStack()
{
    qDebug() << "CullendulaUndoStack::CullendulaUndoStack(): dtor";
}

//----------------------------------------------------------------------------------

void CullendulaUndoStack::push(const QString &from, const QString &to)
{
    //! @todo implement that the container is reset to the position where the "current"-pointer pointed
    while(static_cast<long>(getSize()) - 1 > m_currentItem)
    {
        qDebug() << "push: drop one item"; // todom remove
        m_container.removeLast();
    }

    CullendulaUndoItem newItem(from, to);
    qDebug() << "push: push_back the new item"; // todom remove
    m_container.push_back(newItem);

    // update and initialize the current position
    m_currentItem = static_cast<long>(getSize()) - 1;
}

//----------------------------------------------------------------------------------

CullendulaUndoItem CullendulaUndoStack::undo()
{
    CullendulaUndoItem returnValue = m_currentItem >= 0 ? m_container.value(m_currentItem) : CullendulaUndoItem();
    // decrease the current position
    //! todo

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
