//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#include "Test_CullendulaAppBootstrap.h"

#include <QtWidgets/QApplication>

#include "CullendulaAppBootstrap.h"
#include "CullendulaMainWindow.h"

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

void Test_CullendulaAppBootstrap::slot_Test_SetApplicationLanguage_UpdatesCurrentSelection() {
    QVERIFY(CullendulaAppBootstrap::setApplicationLanguage(CullendulaAppBootstrap::UiLanguage::German));
    QCOMPARE(CullendulaAppBootstrap::getApplicationLanguage(), CullendulaAppBootstrap::UiLanguage::German);

    QVERIFY(CullendulaAppBootstrap::setApplicationLanguage(CullendulaAppBootstrap::UiLanguage::English));
    QCOMPARE(CullendulaAppBootstrap::getApplicationLanguage(), CullendulaAppBootstrap::UiLanguage::English);
}

//----------------------------------------------------------------------------------

void Test_CullendulaAppBootstrap::slot_Test_ShowMainWindow_MakesWindowVisible() {
    CullendulaMainWindow window;

    QVERIFY(!window.isVisible());

    CullendulaAppBootstrap::showMainWindow(window);
    QApplication::processEvents();

    QVERIFY(window.isVisible());
}
