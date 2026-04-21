//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#pragma once

#include <QtTest/QtTest>

/*!
 * @file
 * @brief QTest suite for the GUI bootstrap helper functions.
 */

/*!
 * @brief Covers environment setup and main window presentation helpers.
 */
class Test_CullendulaAppBootstrap : public QObject {
    Q_OBJECT

   private Q_SLOTS:
    //! Verify the helper sets a fallback Qt platform plugin when none exists.
    void slot_Test_EnsureQtPlatformPluginForTests_SetsFallbackWhenUnset();

    //! Verify an explicit Qt platform plugin is preserved.
    void slot_Test_EnsureQtPlatformPluginForTests_RespectsExistingValue();

    //! Verify the helper shows the main window.
    void slot_Test_ShowMainWindow_MakesWindowVisible();
};
