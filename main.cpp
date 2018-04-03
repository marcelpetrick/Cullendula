#include "CullendulaMainWindow.h"

//Qt includes
#include <QApplication>

//----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CullendulaMainWindow w;
    //w.setAcceptDrops(true);
    w.show();

    return a.exec();
}
