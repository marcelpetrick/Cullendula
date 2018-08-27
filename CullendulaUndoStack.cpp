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
}

//----------------------------------------------------------------------------------
