//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#include "CullendulaAppBootstrap.h"

#include <QtCore/QByteArray>
#include <QtCore/QProcessEnvironment>
#include <QtCore/QTimer>
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

bool loadTranslator(QTranslator& translator, QString const& resourcePath) { return translator.load(resourcePath); }

bool installTranslator(QApplication& app, QTranslator* translator) { return app.installTranslator(translator); }

void removeTranslator(QApplication& app, QTranslator* translator) { app.removeTranslator(translator); }
}  // namespace

QString CullendulaAppBootstrap::detail::translationResourcePath(UiLanguage language) {
    switch (language) {
        case UiLanguage::German:
            return QString(":/i18n/Cullendula_de.qm");
        case UiLanguage::Croatian:
            return QString(":/i18n/Cullendula_hr.qm");
        case UiLanguage::Chinese:
            return QString(":/i18n/Cullendula_zh_CN.qm");
        case UiLanguage::English:
            break;
    }

    return {};
}

CullendulaAppBootstrap::detail::TranslatorHooks CullendulaAppBootstrap::detail::defaultTranslatorHooks() {
    return TranslatorHooks{loadTranslator, installTranslator, removeTranslator};
}

bool CullendulaAppBootstrap::detail::setApplicationLanguage(UiLanguage language, QApplication* app, TranslatorHooks const& hooks) {
    if (app == nullptr) {
        return false;
    }

    if (language == UiLanguage::English) {
        if (applicationTranslator()) {
            hooks.remove(*app, applicationTranslator().get());
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
    if (!hooks.load(*nextTranslator, resourcePath)) {
        return false;
    }

    if (applicationTranslator()) {
        hooks.remove(*app, applicationTranslator().get());
    }

    applicationTranslator() = std::move(nextTranslator);
    if (!hooks.install(*app, applicationTranslator().get())) {
        applicationTranslator().reset();
        return false;
    }

    currentApplicationLanguage() = language;
    return true;
}

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
    return detail::setApplicationLanguage(language, qApp, detail::defaultTranslatorHooks());
}

//----------------------------------------------------------------------------------

int CullendulaAppBootstrap::runEventLoop(QApplication& app) {
    Q_UNUSED(app)
    return QApplication::exec();
}

//----------------------------------------------------------------------------------

void CullendulaAppBootstrap::scheduleAutoQuitForTests(QApplication& app) {
    bool parsedSuccessfully = false;
    int const exitDelayMs = qEnvironmentVariableIntValue("CULLENDULA_EXIT_AFTER_STARTUP_MS", &parsedSuccessfully);
    if (!parsedSuccessfully || exitDelayMs < 0) {
        return;
    }

    QTimer::singleShot(exitDelayMs, &app, &QCoreApplication::quit);
}
