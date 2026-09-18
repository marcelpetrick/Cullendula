//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#pragma once

// Qt includes
#include <QtCore/QString>
#include <QtGui/QIcon>

class QApplication;
class CullendulaMainWindow;
class QTranslator;

/*!
 * @file
 * @brief Small startup helpers that keep the GUI bootstrap testable.
 */

namespace CullendulaAppBootstrap {
/*!
 * @brief Supported user-selectable UI languages.
 */
enum class UiLanguage { English, German, Croatian, Chinese };

namespace detail {
/*!
 * @brief Internal hooks used to test translator setup edge cases deterministically.
 */
struct TranslatorHooks {
    bool (*load)(QTranslator&, QString const&);
    bool (*install)(QApplication&, QTranslator*);
    void (*remove)(QApplication&, QTranslator*);
};

/*!
 * @brief Return the translation resource path for a specific UI language.
 * @param language Language whose embedded `.qm` resource should be used.
 * @return Qt resource path, or an empty string for untranslated English.
 */
QString translationResourcePath(UiLanguage language);

/*!
 * @brief Low-level language switch helper used by tests to inject translator behavior.
 * @param language Target language to activate.
 * @param app Application instance that should own the translator, or `nullptr`.
 * @param hooks Translator operations to use for loading and installation.
 * @return `true` when the requested language becomes active.
 */
bool setApplicationLanguage(UiLanguage language, QApplication* app, TranslatorHooks const& hooks);

/*!
 * @brief Return the production translator hooks used by the public bootstrap API.
 */
TranslatorHooks defaultTranslatorHooks();
}  // namespace detail

/*!
 * @brief Force the offscreen Qt platform plugin for headless test runs.
 *
 * Production startup should inherit the desktop platform plugin from the
 * environment. Tests can call this helper to default to the offscreen backend
 * when no explicit plugin was configured yet.
 */
void ensureQtPlatformPluginForTests();

/*!
 * @brief Return the embedded application icon.
 * @return Icon loaded from the Qt resource system.
 *
 * The icon is the same PNG that is installed into the hicolor theme and that the
 * AppImage is packaged with, embedded as a resource so a running window shows it
 * even when the application was started from a build directory.
 */
QIcon applicationIcon();

/*!
 * @brief Apply the embedded application icon to the whole application.
 *
 * Requires a constructed QApplication, because Qt stores the window icon on the
 * application instance. On X11 and Windows this is what a window decoration and a
 * task bar show. On Wayland the compositor resolves the icon through the desktop
 * entry instead, which is matched by the `StartupWMClass` of the shipped
 * `.desktop` file.
 */
void applyApplicationIcon();

/*!
 * @brief Show the main window during application startup.
 * @param mainWindow Main window instance to present to the user.
 */
void showMainWindow(CullendulaMainWindow& mainWindow);

/*!
 * @brief Return the currently installed application language.
 * @return Active UI language selection, with English meaning "no translator".
 */
UiLanguage getApplicationLanguage();

/*!
 * @brief Switch the application UI language at runtime.
 * @param language Target language to activate.
 * @return `true` when the requested language is active after the call.
 *
 * English keeps the untranslated source strings. Other languages are loaded
 * from embedded `.qm` resources generated during the build.
 */
bool setApplicationLanguage(UiLanguage language);

/*!
 * @brief Execute the Qt event loop for the current application instance.
 * @param app QApplication that owns the event loop.
 * @return Process exit code returned by Qt.
 */
int runEventLoop(QApplication const& app);

/*!
 * @brief Optionally schedule an automatic application exit for smoke tests.
 * @param app QApplication whose event loop should be stopped automatically.
 *
 * When the environment variable `CULLENDULA_EXIT_AFTER_STARTUP_MS` is set to a
 * non-negative integer, the application will call `quit()` after that many
 * milliseconds. Normal production runs leave this unset and are unaffected.
 */
void scheduleAutoQuitForTests(QApplication& app);
}  // namespace CullendulaAppBootstrap
