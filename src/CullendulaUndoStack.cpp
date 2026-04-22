//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

// own includes
#include "CullendulaUndoStack.h"

//----------------------------------------------------------------------------------

void CullendulaUndoStack::push(const QString& from, const QString& to) {
    // A new operation invalidates any redo history from an older branch.
    m_redoContainer.clear();
    m_undoContainer.append(CullendulaUndoItem(from, to));
}

//----------------------------------------------------------------------------------

CullendulaUndoItem CullendulaUndoStack::peekUndo() const { return canUndo() ? m_undoContainer.last() : CullendulaUndoItem(); }

//----------------------------------------------------------------------------------

void CullendulaUndoStack::commitUndo() {
    if (!canUndo()) {
        return;
    }

    CullendulaUndoItem const returnValue = m_undoContainer.last();
    m_undoContainer.removeLast();
    m_redoContainer.append(CullendulaUndoItem(returnValue.targetPath, returnValue.sourcePath));
}

//----------------------------------------------------------------------------------

CullendulaUndoItem CullendulaUndoStack::peekRedo() const { return canRedo() ? m_redoContainer.last() : CullendulaUndoItem(); }

//----------------------------------------------------------------------------------

void CullendulaUndoStack::commitRedo() {
    if (!canRedo()) {
        return;
    }

    CullendulaUndoItem const returnValue = m_redoContainer.last();
    m_redoContainer.removeLast();
    m_undoContainer.append(CullendulaUndoItem(returnValue.targetPath, returnValue.sourcePath));
}

//----------------------------------------------------------------------------------

bool CullendulaUndoStack::canUndo() const { return !m_undoContainer.isEmpty(); }

//----------------------------------------------------------------------------------

bool CullendulaUndoStack::canRedo() const { return !m_redoContainer.isEmpty(); }

//----------------------------------------------------------------------------------

long CullendulaUndoStack::getUndoDepth() const { return static_cast<long>(m_undoContainer.size()); }

//----------------------------------------------------------------------------------

long CullendulaUndoStack::getRedoDepth() const { return static_cast<long>(m_redoContainer.size()); }

//----------------------------------------------------------------------------------
