//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#pragma once

// Qt includes
#include <QtCore/QString>

class QApplication;
class CullendulaMainWindow;

/*!
 * @file
 * @brief Small startup helpers that keep the GUI bootstrap testable.
 */

namespace CullendulaAppBootstrap {
/*!
 * @brief Supported user-selectable UI languages.
 */
enum class UiLanguage { English, German, Croatian, Chinese };

/*!
 * @brief Force the offscreen Qt platform plugin for headless test runs.
 *
 * Production startup should inherit the desktop platform plugin from the
 * environment. Tests can call this helper to default to the offscreen backend
 * when no explicit plugin was configured yet.
 */
void ensureQtPlatformPluginForTests();

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
int runEventLoop(QApplication& app);

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
