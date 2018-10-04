//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

// own includes
#include "CullendulaUndoStack.h"

//----------------------------------------------------------------------------------

void CullendulaUndoStack::push(const QString& from, const QString& to)
{
    m_undoContainer.append(CullendulaUndoItem(from, to));
}

//----------------------------------------------------------------------------------

CullendulaUndoItem CullendulaUndoStack::undo()
{
    if(canUndo())
    {
        // pop the first item and return it
        CullendulaUndoItem const returnValue = m_undoContainer.last();
        // remove from this container
        m_undoContainer.removeLast();
        // .. and add to redo: swap order
        m_redoContainer.append(CullendulaUndoItem(returnValue.targetPath, returnValue.sourcePath));

        return returnValue; // conflicts with "single point of exit" ..
    }

    // prevent failure: return a freshly created item
    return CullendulaUndoItem();
}

//----------------------------------------------------------------------------------

CullendulaUndoItem CullendulaUndoStack::redo()
{
    if(canRedo())
    {
        // pop the first item and return it
        CullendulaUndoItem const returnValue = m_redoContainer.last();
        // remove from this container
        m_redoContainer.removeLast();
        // .. and add to undo: swap order
        m_undoContainer.append(CullendulaUndoItem(returnValue.targetPath, returnValue.sourcePath));

        return returnValue; // conflicts with "single point of exit" ..
    }

    // prevent failure: return a freshly created item
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

long CullendulaUndoStack::getUndoDepth()
{
    return static_cast<long>(m_undoContainer.size());
}

//----------------------------------------------------------------------------------

long CullendulaUndoStack::getRedoDepth()
{
    return static_cast<long>(m_redoContainer.size());
}

//----------------------------------------------------------------------------------
