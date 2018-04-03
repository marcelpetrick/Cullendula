
// own includes
#include "CullendulaMainWindow.h"
#include "ui_CullendulaMainWindow.h"

// Qt includes
#include <QtGui/QDropEvent>
#include <QtCore/QMimeData>
#include <QtCore/QList>
#include <QtCore/QDebug> //todom maybe remove

//----------------------------------------------------------------------------

CullendulaMainWindow::CullendulaMainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CullendulaMainWindow),
    m_workingPath("")
{
    setAcceptDrops(true);
    ui->setupUi(this);

//    ui->label->setAcceptDrops(true);
    setAcceptDrops(true);
}

//----------------------------------------------------------------------------

CullendulaMainWindow::~CullendulaMainWindow()
{
    delete ui;
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::dragMoveEvent(QDragMoveEvent* event)
{
    Q_UNUSED(event)
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::dragLeaveEvent(QDragLeaveEvent* event)
{
    Q_UNUSED(event)
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::dropEvent(QDropEvent* event)
{
  const QMimeData* mimeData = event->mimeData();

  // check for our needed mime type, here a file or a list of files
  if (mimeData->hasUrls())
  {
    QStringList pathList;
    QList<QUrl> urlList = mimeData->urls();

    // extract the local paths of the files: print all of them; can be removed laters
    qDebug() << "new drop:";
    for (auto const& url : urlList)
    {
      qDebug() << "\tdropped: " << url;
    }

    // just use the very first one ..
    if(!urlList.isEmpty())
    {
        m_workingPath.setPath(urlList.first().path());

        // trigger now the follow-up
        processNewPath();
    }
  }

  event->acceptProposedAction();
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::processNewPath()
{
    // trim the path ..
    qDebug() << "CullendulaMainWindow::processNewPath():";
    qDebug() << "absolutePath:" << m_workingPath.absolutePath();
    qDebug() << "absoluteFilePath:" << m_workingPath.absoluteFilePath(m_workingPath.path());

    // check existance ..
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::createImageFileList()
{

}

//----------------------------------------------------------------------------

void CullendulaMainWindow::createOutputFolder()
{

}
