//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#include "CullendulaAppBootstrap.h"

#include <QtCore/QByteArray>
#include <QtCore/QProcessEnvironment>
#include <QtCore/QTranslator>
#include <QtWidgets/QApplication>
#include <memory>

#include "CullendulaMainWindow.h"

namespace {
std::unique_ptr<QTranslator>& applicationTranslator() {
    static std::unique_ptr<QTranslator> translator;
    return translator;
}

CullendulaAppBootstrap::UiLanguage& currentApplicationLanguage() {
    static CullendulaAppBootstrap::UiLanguage currentLanguage = CullendulaAppBootstrap::UiLanguage::English;
    return currentLanguage;
}

QString translationResourcePath(CullendulaAppBootstrap::UiLanguage language) {
    switch (language) {
        case CullendulaAppBootstrap::UiLanguage::German:
            return QString(":/i18n/Cullendula_de.qm");
        case CullendulaAppBootstrap::UiLanguage::Croatian:
            return QString(":/i18n/Cullendula_hr.qm");
        case CullendulaAppBootstrap::UiLanguage::Chinese:
            return QString(":/i18n/Cullendula_zh_CN.qm");
        case CullendulaAppBootstrap::UiLanguage::English:
            break;
    }

    return {};
}
}  // namespace

void CullendulaAppBootstrap::ensureQtPlatformPluginForTests() {
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")) {
        qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    }
}

//----------------------------------------------------------------------------------

void CullendulaAppBootstrap::showMainWindow(CullendulaMainWindow& mainWindow) { mainWindow.show(); }

//----------------------------------------------------------------------------------

CullendulaAppBootstrap::UiLanguage CullendulaAppBootstrap::getApplicationLanguage() { return currentApplicationLanguage(); }

//----------------------------------------------------------------------------------

bool CullendulaAppBootstrap::setApplicationLanguage(UiLanguage language) {
    if (qApp == nullptr) {
        return false;
    }

    if (language == UiLanguage::English) {
        if (applicationTranslator()) {
            qApp->removeTranslator(applicationTranslator().get());
            applicationTranslator().reset();
        }
        currentApplicationLanguage() = UiLanguage::English;
        return true;
    }

    QString const resourcePath = translationResourcePath(language);
    if (resourcePath.isEmpty()) {
        return false;
    }

    auto nextTranslator = std::make_unique<QTranslator>();
    if (!nextTranslator->load(resourcePath)) {
        return false;
    }

    if (applicationTranslator()) {
        qApp->removeTranslator(applicationTranslator().get());
    }

    applicationTranslator() = std::move(nextTranslator);
    if (!qApp->installTranslator(applicationTranslator().get())) {
        applicationTranslator().reset();
        return false;
    }

    currentApplicationLanguage() = language;
    return true;
}

//----------------------------------------------------------------------------------

int CullendulaAppBootstrap::runEventLoop(QApplication& app) {
    Q_UNUSED(app)
    return QApplication::exec();
}
