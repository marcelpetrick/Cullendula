//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#include "Test_CullendulaAppBootstrap.h"

#include <QtCore/QElapsedTimer>
#include <QtCore/QFile>
#include <QtCore/QTimer>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>
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

    // The point is that an invalid delay must not quit the loop by itself. Asserting on
    // elapsed wall-clock time made that depend on timer granularity, which is coarse
    // enough on some platforms for a 50 ms timer to read as less than 40 ms. Checking
    // which timer ended the loop tests the actual behaviour instead.
    bool quitCameFromOwnTimer = false;

    CullendulaAppBootstrap::scheduleAutoQuitForTests(*qApp);
    QTimer::singleShot(50, qApp, [&quitCameFromOwnTimer]() {
        quitCameFromOwnTimer = true;
        QCoreApplication::quit();
    });
    QCOMPARE(CullendulaAppBootstrap::runEventLoop(*qApp), 0);

    QVERIFY(quitCameFromOwnTimer);
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

//----------------------------------------------------------------------------------

void Test_CullendulaAppBootstrap::slot_Test_ApplicationIcon_ResolvesEmbeddedResource() {
    // A resource path that does not resolve yields a perfectly valid but empty QIcon, so
    // asserting on the QIcon alone would still pass with a mistyped prefix or a file name
    // the resource compiler stored under a different path. Checking the file and the
    // rendered pixmap is what actually proves the icon ships inside the binary.
    QVERIFY(QFile::exists(QStringLiteral(":/icons/it.marcelpetrick.Cullendula.png")));

    QIcon const icon = CullendulaAppBootstrap::applicationIcon();
    QVERIFY(!icon.isNull());

    QPixmap const pixmap = icon.pixmap(32, 32);
    QVERIFY(!pixmap.isNull());
    QCOMPARE(pixmap.size(), QSize(32, 32));
}

//----------------------------------------------------------------------------------

void Test_CullendulaAppBootstrap::slot_Test_ApplyApplicationIcon_SetsWindowIcon() {
    QIcon const previousIcon = QApplication::windowIcon();
    QApplication::setWindowIcon(QIcon());
    QVERIFY(QApplication::windowIcon().isNull());

    CullendulaAppBootstrap::applyApplicationIcon();

    QVERIFY(!QApplication::windowIcon().isNull());
    QCOMPARE(QApplication::windowIcon().pixmap(32, 32).size(), QSize(32, 32));

    QApplication::setWindowIcon(previousIcon);
}
