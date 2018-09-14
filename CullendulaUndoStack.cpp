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
    m_currentItem = getSize() - 1;
    //m_currentItem = static_cast<long>(getSize()) - 1;
}

//----------------------------------------------------------------------------------

CullendulaUndoStack::~CullendulaUndoStack()
{
    qDebug() << "CullendulaUndoStack::CullendulaUndoStack(): dtor";
}

//----------------------------------------------------------------------------------

void CullendulaUndoStack::push(const QString& from, const QString& to)
{
    // if it does not point to the last element on the stack, then cut off the remaining elements
    if(m_currentItem > 0 && m_currentItem < (getSize() - 1))
    {
        qDebug() << "insert at pos" << m_currentItem << " - remove rest of " << getSize() << "items";
        int const from = m_currentItem;
        int const n = getSize() - m_currentItem;
        m_container.remove(from, n);
    }

    //! @todo implement that the container is reset to the position where the "current"-pointer pointed
//    int compare = static_cast<long>(getSize());
//    while(compare > m_currentItem + 1)
//    {
//        qDebug() << "push: drop one item"; // todom remove
//        m_container.removeLast();
//        compare = static_cast<long>(getSize());
//    }

    CullendulaUndoItem newItem(from, to);
    qDebug() << "push: push_back the new item"; // todom remove
    m_container.push_back(newItem);

    // update and initialize the current position
    m_currentItem = static_cast<long>(getSize()) - 1;
}

//----------------------------------------------------------------------------------

CullendulaUndoItem CullendulaUndoStack::undo()
{
    if(getSize() == 0) // prevent failure
    {
        return CullendulaUndoItem();
    }
    else
    {
        CullendulaUndoItem returnValue = m_container.value(m_currentItem);
        m_currentItem--;
        return returnValue;
    }
}

//----------------------------------------------------------------------------------

CullendulaUndoItem CullendulaUndoStack::redo()
{
    //! @todo
    return CullendulaUndoItem();
}

//----------------------------------------------------------------------------------

bool CullendulaUndoStack::canUndo()
{
    // the container has items
    // and the current item is not the
    //! todo question: what to do with the very first item on the stack?!?
    return (!m_container.isEmpty() && m_currentItem >= 0); //! todo fix this
}

//----------------------------------------------------------------------------------

bool CullendulaUndoStack::canRedo()
{
    // the container has items
    // and the "following" item is not past the amount of items
    return (!m_container.isEmpty() && m_currentItem+1 <= m_container.size()); //! todo check if this is correct - currently just redo changes
}

//----------------------------------------------------------------------------------

long CullendulaUndoStack::getSize()
{
    return static_cast<long>(m_container.size());
}

//----------------------------------------------------------------------------------
