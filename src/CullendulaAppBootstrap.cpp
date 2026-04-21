//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#include "CullendulaAppBootstrap.h"

#include <QtCore/QByteArray>
#include <QtCore/QProcessEnvironment>
#include <QtWidgets/QApplication>

#include "CullendulaMainWindow.h"

void CullendulaAppBootstrap::ensureQtPlatformPluginForTests() {
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")) {
        qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    }
}

//----------------------------------------------------------------------------------

void CullendulaAppBootstrap::showMainWindow(CullendulaMainWindow& mainWindow) { mainWindow.show(); }

//----------------------------------------------------------------------------------

int CullendulaAppBootstrap::runEventLoop(QApplication& app) {
    Q_UNUSED(app)
    return QApplication::exec();
}
