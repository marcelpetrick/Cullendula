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
    //! @todo remove and replace with default
    qDebug() << "CullendulaUndoStack::CullendulaUndoStack(): ctor";
}

//----------------------------------------------------------------------------------

CullendulaUndoStack::~CullendulaUndoStack()
{
    //! @todo remove and replace with default
    qDebug() << "CullendulaUndoStack::CullendulaUndoStack(): dtor";
}

//----------------------------------------------------------------------------------

void CullendulaUndoStack::push(const QString& from, const QString& to)
{
    m_undoContainer.append(CullendulaUndoItem(from, to));

//    CullendulaUndoItem newItem(from, to);
//    qDebug() << "push: push_back the new item"; // todom remove
//    m_undoContainer.push_back(newItem);
}

//----------------------------------------------------------------------------------

CullendulaUndoItem CullendulaUndoStack::undo()
{
    if(canUndo())
    {
        // pop the first item and return it
        CullendulaUndoItem const returnValue = m_undoContainer.last();
        m_undoContainer.removeLast();
        return returnValue;
    }
    else // prevent failure
    {
        // return a freshly created item
        return CullendulaUndoItem();
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
    // the undo-container has items
    return !m_undoContainer.isEmpty();
}

//----------------------------------------------------------------------------------

bool CullendulaUndoStack::canRedo()
{
    // the redo-container has items
    return !m_redoContainer.isEmpty();
}

//----------------------------------------------------------------------------------

long CullendulaUndoStack::getSize()
{
    //! todo still needed?
    return static_cast<long>(m_undoContainer.size());
}

//----------------------------------------------------------------------------------
