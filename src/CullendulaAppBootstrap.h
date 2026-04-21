//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#pragma once

class QApplication;
class CullendulaMainWindow;

/*!
 * @file
 * @brief Small startup helpers that keep the GUI bootstrap testable.
 */

namespace CullendulaAppBootstrap {
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
 * @brief Execute the Qt event loop for the current application instance.
 * @param app QApplication that owns the event loop.
 * @return Process exit code returned by Qt.
 */
int runEventLoop(QApplication& app);
}  // namespace CullendulaAppBootstrap
