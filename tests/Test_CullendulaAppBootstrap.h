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

    //! Verify English is the default application language selection.
    static void slot_Test_DefaultApplicationLanguage_IsEnglish();

    //! Verify English intentionally maps to no translation resource.
    void slot_Test_TranslationResourcePath_EnglishIsEmpty();

    //! Verify each translated language resolves to the expected embedded resource.
    void slot_Test_TranslationResourcePath_TranslatedLanguagesUseExpectedResources();

    //! Verify switching the application language updates the tracked selection.
    static void slot_Test_SetApplicationLanguage_UpdatesCurrentSelection();

    //! Verify invalid language values are rejected by the low-level helper.
    void slot_Test_SetApplicationLanguage_InvalidLanguageIsRejected();

    //! Verify the low-level helper rejects calls without a QApplication.
    static void slot_Test_SetApplicationLanguage_RejectsNullApplication();

    //! Verify translator load failures are surfaced cleanly.
    void slot_Test_SetApplicationLanguage_LoadFailureIsRejected();

    //! Verify translator install failures are surfaced cleanly.
    void slot_Test_SetApplicationLanguage_InstallFailureIsRejected();

    //! Verify a valid auto-quit schedule stops the event loop immediately.
    void slot_Test_ScheduleAutoQuitForTests_ValidDelayQuitsEventLoop();

    //! Verify invalid auto-quit values are ignored.
    void slot_Test_ScheduleAutoQuitForTests_InvalidDelayDoesNothing();

    //! Verify the helper shows the main window.
    void slot_Test_ShowMainWindow_MakesWindowVisible();
};
