#include "CullendulaMainWindow.h"
#include "ui_CullendulaMainWindow.h"

// Qt includes
#include <QDropEvent>
#include <QtCore/QDebug>
#include <QtCore/QMimeData>
#include <QtCore/QList>

//----------------------------------------------------------------------------

CullendulaMainWindow::CullendulaMainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::CullendulaMainWindow)
{
    setAcceptDrops(true);
    ui->setupUi(this);

    //connect(ui->label, &QLabel::dropEvent, this, &CullendulaMainWindow::dropvEvent);

    ui->label->setAcceptDrops(true);
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

void CullendulaMainWindow::dragMoveEvent(QDragMoveEvent *event)
{

}

//----------------------------------------------------------------------------

void CullendulaMainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{

}

//----------------------------------------------------------------------------

//void CullendulaMainWindow::dropEvent(QDropEvent* event)
//{
//    //from the qt example
////    textBrowser->setPlainText(event->mimeData()->text());
////    mimeTypeCombo->clear();
////    mimeTypeCombo->addItems(event->mimeData()->formats());

//    qDebug() << "CullendulaMainWindow::dropEvent"; //todom remove
//    event->acceptProposedAction();
//}

void CullendulaMainWindow::dropEvent(QDropEvent* event)
{
  const QMimeData* mimeData = event->mimeData();

  // check for our needed mime type, here a file or a list of files
  if (mimeData->hasUrls())
  {
    QStringList pathList;
    QList<QUrl> urlList = mimeData->urls();

    // extract the local paths of the files
    for (int i = 0; i < urlList.size() && i < 32; +i)
    {
      pathList.append(urlList.at(i).toLocalFile());
    }

    // call a function to open the files
    //openFiles(pathList);
  }

  qDebug() << "CullendulaMainWindow::dropEvent"; //todom remove
  event->acceptProposedAction();

}
