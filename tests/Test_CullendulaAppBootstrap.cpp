//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#include "Test_CullendulaAppBootstrap.h"

#include <QtCore/QElapsedTimer>
#include <QtCore/QTimer>
#include <QtWidgets/QApplication>

#include "CullendulaAppBootstrap.h"
#include "CullendulaMainWindow.h"

namespace {
bool failingLoad(QTranslator&, QString const&) { return false; }

bool successfulInstall(QApplication&, QTranslator*) { return true; }

bool failingInstall(QApplication&, QTranslator*) { return false; }

void removeTranslator(QApplication&, QTranslator*) {}
}  // namespace

void Test_CullendulaAppBootstrap::slot_Test_EnsureQtPlatformPluginForTests_SetsFallbackWhenUnset() {
    qunsetenv("QT_QPA_PLATFORM");

    CullendulaAppBootstrap::ensureQtPlatformPluginForTests();

    QCOMPARE(qgetenv("QT_QPA_PLATFORM"), QByteArray("offscreen"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaAppBootstrap::slot_Test_EnsureQtPlatformPluginForTests_RespectsExistingValue() {
    qputenv("QT_QPA_PLATFORM", QByteArray("wayland"));

    CullendulaAppBootstrap::ensureQtPlatformPluginForTests();

    QCOMPARE(qgetenv("QT_QPA_PLATFORM"), QByteArray("wayland"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaAppBootstrap::slot_Test_DefaultApplicationLanguage_IsEnglish() {
    QCOMPARE(CullendulaAppBootstrap::getApplicationLanguage(), CullendulaAppBootstrap::UiLanguage::English);
}

//----------------------------------------------------------------------------------

void Test_CullendulaAppBootstrap::slot_Test_TranslationResourcePath_EnglishIsEmpty() {
    QCOMPARE(CullendulaAppBootstrap::detail::translationResourcePath(CullendulaAppBootstrap::UiLanguage::English), QString());
}

//----------------------------------------------------------------------------------

void Test_CullendulaAppBootstrap::slot_Test_TranslationResourcePath_TranslatedLanguagesUseExpectedResources() {
    QCOMPARE(CullendulaAppBootstrap::detail::translationResourcePath(CullendulaAppBootstrap::UiLanguage::German), QString(":/i18n/Cullendula_de.qm"));
    QCOMPARE(CullendulaAppBootstrap::detail::translationResourcePath(CullendulaAppBootstrap::UiLanguage::Croatian), QString(":/i18n/Cullendula_hr.qm"));
    QCOMPARE(CullendulaAppBootstrap::detail::translationResourcePath(CullendulaAppBootstrap::UiLanguage::Chinese), QString(":/i18n/Cullendula_zh_CN.qm"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaAppBootstrap::slot_Test_SetApplicationLanguage_UpdatesCurrentSelection() {
    QVERIFY(CullendulaAppBootstrap::setApplicationLanguage(CullendulaAppBootstrap::UiLanguage::German));
    QCOMPARE(CullendulaAppBootstrap::getApplicationLanguage(), CullendulaAppBootstrap::UiLanguage::German);

    QVERIFY(CullendulaAppBootstrap::setApplicationLanguage(CullendulaAppBootstrap::UiLanguage::English));
    QCOMPARE(CullendulaAppBootstrap::getApplicationLanguage(), CullendulaAppBootstrap::UiLanguage::English);
}

//----------------------------------------------------------------------------------

void Test_CullendulaAppBootstrap::slot_Test_SetApplicationLanguage_InvalidLanguageIsRejected() {
    QVERIFY(CullendulaAppBootstrap::setApplicationLanguage(CullendulaAppBootstrap::UiLanguage::German));

    QVERIFY(!CullendulaAppBootstrap::detail::setApplicationLanguage(static_cast<CullendulaAppBootstrap::UiLanguage>(999), qApp,
                                                                    CullendulaAppBootstrap::detail::defaultTranslatorHooks()));
    QCOMPARE(CullendulaAppBootstrap::getApplicationLanguage(), CullendulaAppBootstrap::UiLanguage::German);

    QVERIFY(CullendulaAppBootstrap::setApplicationLanguage(CullendulaAppBootstrap::UiLanguage::English));
}

//----------------------------------------------------------------------------------

void Test_CullendulaAppBootstrap::slot_Test_SetApplicationLanguage_RejectsNullApplication() {
    QVERIFY(!CullendulaAppBootstrap::detail::setApplicationLanguage(CullendulaAppBootstrap::UiLanguage::German, nullptr,
                                                                    CullendulaAppBootstrap::detail::defaultTranslatorHooks()));
}

//----------------------------------------------------------------------------------

void Test_CullendulaAppBootstrap::slot_Test_SetApplicationLanguage_LoadFailureIsRejected() {
    CullendulaAppBootstrap::detail::TranslatorHooks const hooks{failingLoad, successfulInstall, removeTranslator};

    QVERIFY(!CullendulaAppBootstrap::detail::setApplicationLanguage(CullendulaAppBootstrap::UiLanguage::German, qApp, hooks));
    QCOMPARE(CullendulaAppBootstrap::getApplicationLanguage(), CullendulaAppBootstrap::UiLanguage::English);
}

//----------------------------------------------------------------------------------

void Test_CullendulaAppBootstrap::slot_Test_SetApplicationLanguage_InstallFailureIsRejected() {
    CullendulaAppBootstrap::detail::TranslatorHooks const hooks{CullendulaAppBootstrap::detail::defaultTranslatorHooks().load, failingInstall,
                                                                removeTranslator};

    QVERIFY(!CullendulaAppBootstrap::detail::setApplicationLanguage(CullendulaAppBootstrap::UiLanguage::German, qApp, hooks));
    QCOMPARE(CullendulaAppBootstrap::getApplicationLanguage(), CullendulaAppBootstrap::UiLanguage::English);
}

//----------------------------------------------------------------------------------

void Test_CullendulaAppBootstrap::slot_Test_ScheduleAutoQuitForTests_ValidDelayQuitsEventLoop() {
    qputenv("CULLENDULA_EXIT_AFTER_STARTUP_MS", QByteArray("0"));

    QElapsedTimer timer;
    timer.start();

    CullendulaAppBootstrap::scheduleAutoQuitForTests(*qApp);
    QCOMPARE(CullendulaAppBootstrap::runEventLoop(*qApp), 0);

    QVERIFY(timer.elapsed() < 100);
    qunsetenv("CULLENDULA_EXIT_AFTER_STARTUP_MS");
}

//----------------------------------------------------------------------------------

void Test_CullendulaAppBootstrap::slot_Test_ScheduleAutoQuitForTests_InvalidDelayDoesNothing() {
    qputenv("CULLENDULA_EXIT_AFTER_STARTUP_MS", QByteArray("-1"));

    QElapsedTimer timer;
    timer.start();

    CullendulaAppBootstrap::scheduleAutoQuitForTests(*qApp);
    QTimer::singleShot(50, qApp, &QCoreApplication::quit);
    QCOMPARE(CullendulaAppBootstrap::runEventLoop(*qApp), 0);

    QVERIFY(timer.elapsed() >= 40);
    qunsetenv("CULLENDULA_EXIT_AFTER_STARTUP_MS");
}

//----------------------------------------------------------------------------------

void Test_CullendulaAppBootstrap::slot_Test_ShowMainWindow_MakesWindowVisible() {
    CullendulaMainWindow window;

    QVERIFY(!window.isVisible());

    CullendulaAppBootstrap::showMainWindow(window);
    QApplication::processEvents();

    QVERIFY(window.isVisible());
}
