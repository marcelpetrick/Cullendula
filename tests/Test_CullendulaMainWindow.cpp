//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#include "Test_CullendulaMainWindow.h"

#include <QtCore/QFile>
#include <QtCore/QMimeData>
#include <QtCore/QRegularExpression>
#include <QtGui/QAction>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QImage>
#include <QtGui/QPalette>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>

//----------------------------------------------------------------------------------

namespace {
class TestableCullendulaMainWindow : public CullendulaMainWindow {
   public:
    using CullendulaMainWindow::CullendulaMainWindow;
    using CullendulaMainWindow::dragEnterEvent;
    using CullendulaMainWindow::dropEvent;
};
}  // namespace

//----------------------------------------------------------------------------------

QString Test_CullendulaMainWindow::createImage(QString const& relativePath, QColor const& color) {
    QString const absolutePath = m_tempDir->path() + QDir::separator() + relativePath;
    QFileInfo const fileInfo(absolutePath);
    QDir().mkpath(fileInfo.absolutePath());

    QImage image(24, 24, QImage::Format_RGB32);
    image.fill(color);
    if (!image.save(absolutePath)) {
        QTest::qFail("Could not create test image", __FILE__, __LINE__);
        return {};
    }

    return absolutePath;
}

//----------------------------------------------------------------------------------

QString Test_CullendulaMainWindow::createInvalidImageFile(QString const& relativePath) {
    QString const absolutePath = m_tempDir->path() + QDir::separator() + relativePath;
    QFileInfo const fileInfo(absolutePath);
    QDir().mkpath(fileInfo.absolutePath());

    QFile file(absolutePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QTest::qFail("Could not create invalid image test file", __FILE__, __LINE__);
        return {};
    }

    file.write("not a real image");
    file.close();
    return absolutePath;
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::sendDropWithUrls(QList<QUrl> const& urls) {
    QMimeData mimeData;
    mimeData.setUrls(urls);

    QDragEnterEvent dragEnterEvent(QPoint(10, 10), Qt::CopyAction, &mimeData, Qt::LeftButton, Qt::NoModifier);
    static_cast<TestableCullendulaMainWindow*>(m_window.get())->dragEnterEvent(&dragEnterEvent);

    QDropEvent dropEvent(QPointF(10, 10), Qt::CopyAction, &mimeData, Qt::LeftButton, Qt::NoModifier);
    static_cast<TestableCullendulaMainWindow*>(m_window.get())->dropEvent(&dropEvent);
    QApplication::processEvents();
}

//----------------------------------------------------------------------------------

QAction* Test_CullendulaMainWindow::findAction(QString const& text) const {
    QList<QAction*> const actions = m_window->findChildren<QAction*>();
    for (QAction* action : actions) {
        if (action->text() == text) {
            return action;
        }
    }

    return nullptr;
}

//----------------------------------------------------------------------------------

QAction* Test_CullendulaMainWindow::findExtensionAction(QString const& extension) const {
    return m_window->findChild<QAction*>("extensionAction_" + extension.toLower());
}

//----------------------------------------------------------------------------------

QAction* Test_CullendulaMainWindow::findThemeAction(QString const& themeName) const {
    return m_window->findChild<QAction*>("themeAction_" + themeName.toLower());
}

//----------------------------------------------------------------------------------

QAction* Test_CullendulaMainWindow::findLanguageAction(QString const& languageCode) const {
    return m_window->findChild<QAction*>("languageAction_" + languageCode);
}

//----------------------------------------------------------------------------------

QDialog* Test_CullendulaMainWindow::findOpenDialog() const {
    QList<QWidget*> const topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget* widget : topLevelWidgets) {
        if (widget == m_window.get()) {
            continue;
        }

        if (auto* dialog = qobject_cast<QDialog*>(widget); dialog != nullptr && dialog->isVisible()) {
            return dialog;
        }
    }

    return nullptr;
}

//----------------------------------------------------------------------------------

QPushButton* Test_CullendulaMainWindow::findButton(char const* name) const { return m_window->findChild<QPushButton*>(name); }

//----------------------------------------------------------------------------------

QLabel* Test_CullendulaMainWindow::findCenterLabel() const { return m_window->findChild<QLabel*>("centerLabel"); }

//----------------------------------------------------------------------------------

QStatusBar* Test_CullendulaMainWindow::findStatusBar() const { return m_window->findChild<QStatusBar*>("statusBar"); }

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::init() {
    m_tempDir = std::make_unique<QTemporaryDir>();
    QVERIFY(m_tempDir->isValid());

    m_window = std::make_unique<TestableCullendulaMainWindow>();
    m_window->resize(640, 480);
    m_window->show();
    QApplication::processEvents();
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::cleanup() {
    m_window.reset();
    m_tempDir.reset();
    QVERIFY(CullendulaAppBootstrap::setApplicationLanguage(CullendulaAppBootstrap::UiLanguage::English));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_InitialState() {
    QVERIFY(m_window->windowTitle().startsWith("Cullendula - v"));
    QVERIFY(!findButton("leftPB")->isEnabled());
    QVERIFY(!findButton("rightPB")->isEnabled());
    QVERIFY(!findButton("savePB")->isEnabled());
    QVERIFY(!findButton("trashPB")->isEnabled());
    QVERIFY(findAction("Undo") != nullptr);
    QVERIFY(findAction("Redo") != nullptr);
    QVERIFY(findAction("About Cullendula") != nullptr);
    QVERIFY(findAction("About Qt") != nullptr);
    QVERIFY(!findAction("Undo")->isEnabled());
    QVERIFY(!findAction("Redo")->isEnabled());
    QVERIFY(findCenterLabel()->text().contains("no more valid images found"));
    QCOMPARE(findStatusBar()->currentMessage(), QString("no more files"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_VersionMetadata_IsDocumentedConsistently() {
    QFile cmakeFile(QStringLiteral("/home/mpetrick/repos/Cullendula/CMakeLists.txt"));
    QVERIFY(cmakeFile.open(QIODevice::ReadOnly | QIODevice::Text));
    QString const cmakeContents = QString::fromUtf8(cmakeFile.readAll());
    QVERIFY(cmakeContents.contains("VERSION 0.6.25"));

    QFile readmeFile(QStringLiteral("/home/mpetrick/repos/Cullendula/README.md"));
    QVERIFY(readmeFile.open(QIODevice::ReadOnly | QIODevice::Text));
    QString const readmeContents = QString::fromUtf8(readmeFile.readAll());
    QVERIFY(readmeContents.contains("This is version 0.6.25."));
    QVERIFY(readmeContents.contains("* v0.6.25 prepares the German, Croatian, and Chinese translations"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_LightTheme_IsDefault() {
    QAction* lightThemeAction = findThemeAction("light");
    QAction* darkThemeAction = findThemeAction("dark");

    QVERIFY(lightThemeAction != nullptr);
    QVERIFY(darkThemeAction != nullptr);
    QVERIFY(lightThemeAction->isCheckable());
    QVERIFY(darkThemeAction->isCheckable());
    QVERIFY(lightThemeAction->isChecked());
    QVERIFY(!darkThemeAction->isChecked());
    QCOMPARE(m_window->getThemeMode(), CullendulaMainWindow::ThemeMode::Light);
    QVERIFY(qApp->styleSheet().contains("#f6f3ee"));
    QCOMPARE(qApp->palette().color(QPalette::Window), QColor("#f6f3ee"));
    QCOMPARE(qApp->palette().color(QPalette::Button), QColor("#efe2cc"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_ThemeMenu_SwitchesToDarkMode() {
    QAction* lightThemeAction = findThemeAction("light");
    QAction* darkThemeAction = findThemeAction("dark");
    QVERIFY(lightThemeAction != nullptr);
    QVERIFY(darkThemeAction != nullptr);

    darkThemeAction->trigger();
    QApplication::processEvents();

    QVERIFY(!lightThemeAction->isChecked());
    QVERIFY(darkThemeAction->isChecked());
    QCOMPARE(m_window->getThemeMode(), CullendulaMainWindow::ThemeMode::Dark);
    QVERIFY(qApp->styleSheet().contains("#0b0f14"));
    QVERIFY(qApp->styleSheet().contains("#79c0ff"));
    QCOMPARE(qApp->palette().color(QPalette::Window), QColor("#0b0f14"));
    QCOMPARE(qApp->palette().color(QPalette::Button), QColor("#16324b"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_ThemeMenu_SwitchesBackToLightMode() {
    QAction* lightThemeAction = findThemeAction("light");
    QAction* darkThemeAction = findThemeAction("dark");
    QVERIFY(lightThemeAction != nullptr);
    QVERIFY(darkThemeAction != nullptr);

    darkThemeAction->trigger();
    QApplication::processEvents();
    lightThemeAction->trigger();
    QApplication::processEvents();

    QVERIFY(lightThemeAction->isChecked());
    QVERIFY(!darkThemeAction->isChecked());
    QCOMPARE(m_window->getThemeMode(), CullendulaMainWindow::ThemeMode::Light);
    QCOMPARE(qApp->palette().color(QPalette::Window), QColor("#f6f3ee"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_LanguageMenu_ContainsSupportedLanguages() {
    QAction* englishAction = findLanguageAction("en");
    QAction* germanAction = findLanguageAction("de");
    QAction* croatianAction = findLanguageAction("hr");
    QAction* chineseAction = findLanguageAction("zh_CN");

    QVERIFY(englishAction != nullptr);
    QVERIFY(germanAction != nullptr);
    QVERIFY(croatianAction != nullptr);
    QVERIFY(chineseAction != nullptr);
    QVERIFY(englishAction->isCheckable());
    QVERIFY(germanAction->isCheckable());
    QVERIFY(croatianAction->isCheckable());
    QVERIFY(chineseAction->isCheckable());
    QVERIFY(englishAction->isChecked());
    QVERIFY(!germanAction->isChecked());
    QVERIFY(!croatianAction->isChecked());
    QVERIFY(!chineseAction->isChecked());
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_LanguageMenu_SwitchesCurrentLanguage() {
    QAction* englishAction = findLanguageAction("en");
    QAction* germanAction = findLanguageAction("de");
    QAction* croatianAction = findLanguageAction("hr");
    QAction* chineseAction = findLanguageAction("zh_CN");
    QVERIFY(englishAction != nullptr);
    QVERIFY(germanAction != nullptr);
    QVERIFY(croatianAction != nullptr);
    QVERIFY(chineseAction != nullptr);

    germanAction->trigger();
    QApplication::processEvents();
    QVERIFY(germanAction->isChecked());
    QVERIFY(!englishAction->isChecked());
    QCOMPARE(CullendulaAppBootstrap::getApplicationLanguage(), CullendulaAppBootstrap::UiLanguage::German);

    croatianAction->trigger();
    QApplication::processEvents();
    QVERIFY(croatianAction->isChecked());
    QVERIFY(!germanAction->isChecked());
    QCOMPARE(CullendulaAppBootstrap::getApplicationLanguage(), CullendulaAppBootstrap::UiLanguage::Croatian);

    chineseAction->trigger();
    QApplication::processEvents();
    QVERIFY(chineseAction->isChecked());
    QVERIFY(!croatianAction->isChecked());
    QCOMPARE(CullendulaAppBootstrap::getApplicationLanguage(), CullendulaAppBootstrap::UiLanguage::Chinese);

    englishAction->trigger();
    QApplication::processEvents();
    QVERIFY(englishAction->isChecked());
    QVERIFY(!chineseAction->isChecked());
    QCOMPARE(CullendulaAppBootstrap::getApplicationLanguage(), CullendulaAppBootstrap::UiLanguage::English);
    QVERIFY(m_window->windowTitle().startsWith("Cullendula - v"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_ExtensionsMenu_DefaultsToAllChecked() {
    QAction* pngAction = findExtensionAction("png");
    QAction* jpgAction = findExtensionAction("jpg");
    QAction* webpAction = findExtensionAction("webp");

    QVERIFY(pngAction != nullptr);
    QVERIFY(jpgAction != nullptr);
    QVERIFY(webpAction != nullptr);
    QVERIFY(pngAction->isCheckable());
    QVERIFY(jpgAction->isCheckable());
    QVERIFY(webpAction->isCheckable());
    QVERIFY(pngAction->isChecked());
    QVERIFY(jpgAction->isChecked());
    QVERIFY(webpAction->isChecked());
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_ExtensionsMenu_AllUncheckedBlocksNextDrop() {
    createImage("alpha.jpg", Qt::red);
    createImage("beta.png", Qt::blue);

    QList<QAction*> const extensionActions = m_window->findChildren<QAction*>(QRegularExpression("^extensionAction_"));
    QVERIFY(!extensionActions.isEmpty());

    for (QAction* action : extensionActions) {
        QVERIFY(action->isChecked());
        action->setChecked(false);
    }

    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path())});

    QVERIFY(!findButton("leftPB")->isEnabled());
    QVERIFY(!findButton("rightPB")->isEnabled());
    QVERIFY(!findButton("savePB")->isEnabled());
    QVERIFY(!findButton("trashPB")->isEnabled());
    QVERIFY(findCenterLabel()->text().contains("no more valid images found"));
    QCOMPARE(findStatusBar()->currentMessage(), QString("no more files"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_ResizeWithoutLoadedImages_ShowsFallbackText() {
    m_window->resize(800, 520);
    QApplication::processEvents();

    QVERIFY(findCenterLabel()->text().contains("no more valid images found"));
    QVERIFY(!findButton("savePB")->isEnabled());
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_ResizeLoadedImage_ReusesCachedPreview() {
    createImage("alpha.jpg", Qt::red);
    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path())});

    QPixmap const beforeResize = findCenterLabel()->pixmap(Qt::ReturnByValue);
    QVERIFY(!beforeResize.isNull());
    QVERIFY(findStatusBar()->currentMessage().contains("alpha.jpg"));

    m_window->resize(900, 700);
    QApplication::processEvents();

    QPixmap const afterResize = findCenterLabel()->pixmap(Qt::ReturnByValue);
    QVERIFY(!afterResize.isNull());
    QVERIFY(findStatusBar()->currentMessage().contains("alpha.jpg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_InvalidImagePreview_ShowsFallbackError() {
    createInvalidImageFile("broken.jpg");
    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path())});

    QCOMPARE(findCenterLabel()->text(), QString("could not load the current image preview"));
    QCOMPARE(findStatusBar()->currentMessage(), QString("could not load the current image preview"));
    QVERIFY(!findButton("savePB")->isEnabled());
    QVERIFY(!findButton("trashPB")->isEnabled());
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_DragEnter_ValidUrlsAreAccepted() {
    createImage("alpha.jpg", Qt::red);

    QMimeData mimeData;
    mimeData.setUrls({QUrl::fromLocalFile(m_tempDir->path())});

    QDragEnterEvent dragEnterEvent(QPoint(10, 10), Qt::CopyAction, &mimeData, Qt::LeftButton, Qt::NoModifier);
    static_cast<TestableCullendulaMainWindow*>(m_window.get())->dragEnterEvent(&dragEnterEvent);

    QVERIFY(dragEnterEvent.isAccepted());
    QCOMPARE(findStatusBar()->currentMessage(), QString("drop current load and let's see what you dragged?"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_DragEnter_InvalidPayloadIsRejected() {
    QMimeData mimeData;
    mimeData.setText("not a file");

    QDragEnterEvent dragEnterEvent(QPoint(10, 10), Qt::CopyAction, &mimeData, Qt::LeftButton, Qt::NoModifier);
    static_cast<TestableCullendulaMainWindow*>(m_window.get())->dragEnterEvent(&dragEnterEvent);

    QVERIFY(!dragEnterEvent.isAccepted());
    QCOMPARE(findStatusBar()->currentMessage(), QString("no more files"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_DragEnter_EmptyUrlListIsRejected() {
    QMimeData mimeData;
    mimeData.setUrls({});

    QDragEnterEvent dragEnterEvent(QPoint(10, 10), Qt::CopyAction, &mimeData, Qt::LeftButton, Qt::NoModifier);
    static_cast<TestableCullendulaMainWindow*>(m_window.get())->dragEnterEvent(&dragEnterEvent);

    QVERIFY(!dragEnterEvent.isAccepted());
    QCOMPARE(findStatusBar()->currentMessage(), QString("no more files"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_DragEnterAndDropValidDirectory_LoadsImages() {
    createImage("alpha.jpg", Qt::red);
    createImage("beta.jpeg", Qt::blue);

    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path())});

    QVERIFY(findButton("leftPB")->isEnabled());
    QVERIFY(findButton("rightPB")->isEnabled());
    QVERIFY(findButton("savePB")->isEnabled());
    QVERIFY(findButton("trashPB")->isEnabled());
    QVERIFY(!findCenterLabel()->pixmap(Qt::ReturnByValue).isNull());
    QVERIFY(findStatusBar()->currentMessage().contains("showing 1 of 2"));
    QVERIFY(findStatusBar()->currentMessage().contains("alpha.jpg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_ExtensionsMenu_AffectsNextDroppedDirectory() {
    createImage("dir1/alpha.jpg", Qt::red);
    createImage("dir1/beta.png", Qt::blue);
    createImage("dir2/alpha.jpg", Qt::green);
    createImage("dir2/beta.png", Qt::yellow);

    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path() + QDir::separator() + "dir1")});
    QVERIFY(findStatusBar()->currentMessage().contains("alpha.jpg"));

    QAction* jpgAction = findExtensionAction("jpg");
    QAction* jpegAction = findExtensionAction("jpeg");
    QAction* pngAction = findExtensionAction("png");
    QVERIFY(jpgAction != nullptr);
    QVERIFY(jpegAction != nullptr);
    QVERIFY(pngAction != nullptr);

    jpgAction->setChecked(false);
    jpegAction->setChecked(false);
    QVERIFY(pngAction->isChecked());

    QVERIFY(findStatusBar()->currentMessage().contains("alpha.jpg"));

    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path() + QDir::separator() + "dir2")});
    QVERIFY(findStatusBar()->currentMessage().contains("showing 1 of 1"));
    QVERIFY(findStatusBar()->currentMessage().contains("beta.png"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_DroppingEmptyDirectory_ClearsPreviousSessionState() {
    createImage("loaded/alpha.jpg", Qt::red);
    createImage("loaded/beta.jpeg", Qt::blue);
    QDir().mkpath(m_tempDir->path() + QDir::separator() + "empty");

    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path() + QDir::separator() + "loaded")});
    QVERIFY(findButton("savePB")->isEnabled());
    QVERIFY(findStatusBar()->currentMessage().contains("alpha.jpg"));

    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path() + QDir::separator() + "empty")});
    QVERIFY(!findButton("leftPB")->isEnabled());
    QVERIFY(!findButton("rightPB")->isEnabled());
    QVERIFY(!findButton("savePB")->isEnabled());
    QVERIFY(!findButton("trashPB")->isEnabled());
    QVERIFY(findCenterLabel()->text().contains("no more valid images found"));
    QCOMPARE(findStatusBar()->currentMessage(), QString("no more files"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_DroppingDirectoryWithBlockedOutput_ShowsFilesystemError() {
    createImage("loaded/alpha.jpg", Qt::red);

    QFile blockedOutput(m_tempDir->path() + QDir::separator() + "loaded" + QDir::separator() + "output");
    QVERIFY(blockedOutput.open(QIODevice::WriteOnly));
    blockedOutput.write("blocked");
    blockedOutput.close();

    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path() + QDir::separator() + "loaded")});

    QVERIFY(findStatusBar()->currentMessage().contains("Could not prepare 'output' directory"));
    QVERIFY(findStatusBar()->currentMessage().contains("non-directory"));
    QVERIFY(!findButton("savePB")->isEnabled());
    QVERIFY(findCenterLabel()->text().contains("no more valid images found"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_DropInvalidPayload_ShowsErrorStatus() {
    QMimeData mimeData;
    mimeData.setText("not a file");
    QDragEnterEvent dragEnterEvent(QPoint(10, 10), Qt::CopyAction, &mimeData, Qt::LeftButton, Qt::NoModifier);
    QDropEvent dropEvent(QPointF(10, 10), Qt::CopyAction, &mimeData, Qt::LeftButton, Qt::NoModifier);
    auto* window = static_cast<TestableCullendulaMainWindow*>(m_window.get());
    window->dragEnterEvent(&dragEnterEvent);
    window->dropEvent(&dropEvent);
    QApplication::processEvents();

    QCOMPARE(findStatusBar()->currentMessage(), QString("The load was not usable! :("));
    QVERIFY(!findButton("savePB")->isEnabled());
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_DropEmptyUrlList_LeavesStateUnchanged() {
    createImage("alpha.jpg", Qt::red);
    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path())});
    QString const statusBeforeDrop = findStatusBar()->currentMessage();

    QMimeData mimeData;
    mimeData.setUrls({});

    QDragEnterEvent dragEnterEvent(QPoint(10, 10), Qt::CopyAction, &mimeData, Qt::LeftButton, Qt::NoModifier);
    QDropEvent dropEvent(QPointF(10, 10), Qt::CopyAction, &mimeData, Qt::LeftButton, Qt::NoModifier);
    auto* window = static_cast<TestableCullendulaMainWindow*>(m_window.get());
    window->dragEnterEvent(&dragEnterEvent);
    window->dropEvent(&dropEvent);
    QApplication::processEvents();

    QVERIFY(dropEvent.isAccepted());
    QCOMPARE(findStatusBar()->currentMessage(), statusBeforeDrop);
    QVERIFY(findButton("savePB")->isEnabled());
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_DropMultipleUrls_UsesFirstEntry() {
    createImage("first/alpha.jpg", Qt::red);
    createImage("second/beta.jpeg", Qt::blue);

    sendDropWithUrls(
        {QUrl::fromLocalFile(m_tempDir->path() + QDir::separator() + "second"), QUrl::fromLocalFile(m_tempDir->path() + QDir::separator() + "first")});

    QVERIFY(findStatusBar()->currentMessage().contains("beta.jpeg"));
    QVERIFY(!findStatusBar()->currentMessage().contains("alpha.jpg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_ButtonNavigationWithoutSession_LeavesUiUnchanged() {
    QString const initialStatus = findStatusBar()->currentMessage();

    QVERIFY(QMetaObject::invokeMethod(m_window.get(), "slotButtonLeftTriggered"));
    QVERIFY(QMetaObject::invokeMethod(m_window.get(), "slotButtonRightTriggered"));
    QApplication::processEvents();

    QCOMPARE(findStatusBar()->currentMessage(), initialStatus);
    QVERIFY(!findButton("leftPB")->isEnabled());
    QVERIFY(!findButton("rightPB")->isEnabled());
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_ButtonNavigationAndSaveFlow() {
    createImage("alpha.jpg", Qt::red);
    createImage("beta.jpeg", Qt::blue);
    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path())});

    QTest::mouseClick(findButton("rightPB"), Qt::LeftButton);
    QVERIFY(findStatusBar()->currentMessage().contains("beta.jpeg"));

    QTest::mouseClick(findButton("savePB"), Qt::LeftButton);
    QVERIFY(QFile::exists(m_tempDir->path() + QDir::separator() + "output" + QDir::separator() + "beta.jpeg"));
    QVERIFY(findAction("Undo")->isEnabled());
    QVERIFY(!findAction("Redo")->isEnabled());
    QVERIFY(findStatusBar()->currentMessage().contains("showing 1 of 1"));
    QVERIFY(findStatusBar()->currentMessage().contains("alpha.jpg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_ButtonTrashFlow() {
    createImage("alpha.jpg", Qt::red);
    createImage("beta.jpeg", Qt::blue);
    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path())});

    QTest::mouseClick(findButton("trashPB"), Qt::LeftButton);

    QVERIFY(QFile::exists(m_tempDir->path() + QDir::separator() + "trash" + QDir::separator() + "alpha.jpg"));
    QVERIFY(findAction("Undo")->isEnabled());
    QVERIFY(!findAction("Redo")->isEnabled());
    QVERIFY(findStatusBar()->currentMessage().contains("showing 1 of 1"));
    QVERIFY(findStatusBar()->currentMessage().contains("beta.jpeg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_UndoRedoActions_MoveFilesOnDisk() {
    createImage("alpha.jpg", Qt::red);
    createImage("beta.jpeg", Qt::blue);
    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path())});

    QTest::mouseClick(findButton("savePB"), Qt::LeftButton);
    QString const originalPath = m_tempDir->path() + QDir::separator() + "alpha.jpg";
    QString const movedPath = m_tempDir->path() + QDir::separator() + "output" + QDir::separator() + "alpha.jpg";

    QVERIFY(QFile::exists(movedPath));
    QVERIFY(findAction("Undo")->isEnabled());

    findAction("Undo")->trigger();
    QApplication::processEvents();
    QVERIFY(QFile::exists(originalPath));
    QVERIFY(!QFile::exists(movedPath));
    QVERIFY(!findAction("Undo")->isEnabled());
    QVERIFY(findAction("Redo")->isEnabled());
    QVERIFY(findStatusBar()->currentMessage().contains("showing 1 of 2"));
    QVERIFY(findStatusBar()->currentMessage().contains("alpha.jpg"));
    QVERIFY(findButton("leftPB")->isEnabled());
    QVERIFY(findButton("rightPB")->isEnabled());
    QVERIFY(findButton("savePB")->isEnabled());
    QVERIFY(findButton("trashPB")->isEnabled());

    findAction("Redo")->trigger();
    QApplication::processEvents();
    QVERIFY(!QFile::exists(originalPath));
    QVERIFY(QFile::exists(movedPath));
    QVERIFY(findAction("Undo")->isEnabled());
    QVERIFY(!findAction("Redo")->isEnabled());
    QVERIFY(findStatusBar()->currentMessage().contains("showing 1 of 1"));
    QVERIFY(findStatusBar()->currentMessage().contains("beta.jpeg"));
    QVERIFY(findButton("leftPB")->isEnabled());
    QVERIFY(findButton("rightPB")->isEnabled());
    QVERIFY(findButton("savePB")->isEnabled());
    QVERIFY(findButton("trashPB")->isEnabled());
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_UndoFailure_ShowsStatusMessageAndKeepsRedoUnavailable() {
    createImage("alpha.jpg", Qt::red);
    createImage("beta.jpeg", Qt::blue);
    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path())});

    QTest::mouseClick(findButton("savePB"), Qt::LeftButton);
    QString const movedPath = m_tempDir->path() + QDir::separator() + "output" + QDir::separator() + "alpha.jpg";
    QVERIFY(QFile::exists(movedPath));
    QVERIFY(QFile::remove(movedPath));

    findAction("Undo")->trigger();
    QApplication::processEvents();

    QVERIFY(findStatusBar()->currentMessage().contains("Could not move 'alpha.jpg' to 'undo'"));
    QVERIFY(findAction("Undo")->isEnabled());
    QVERIFY(!findAction("Redo")->isEnabled());
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_RedoFailure_ShowsStatusMessageAndKeepsRedoAvailable() {
    createImage("alpha.jpg", Qt::red);
    createImage("beta.jpeg", Qt::blue);
    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path())});

    QTest::mouseClick(findButton("savePB"), Qt::LeftButton);
    findAction("Undo")->trigger();
    QApplication::processEvents();

    QString const restoredPath = m_tempDir->path() + QDir::separator() + "alpha.jpg";
    QVERIFY(QFile::exists(restoredPath));
    QVERIFY(QFile::remove(restoredPath));

    findAction("Redo")->trigger();
    QApplication::processEvents();

    QVERIFY(findStatusBar()->currentMessage().contains("Could not move 'alpha.jpg' to 'redo'"));
    QVERIFY(!findAction("Undo")->isEnabled());
    QVERIFY(findAction("Redo")->isEnabled());
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_SaveFailure_ShowsStatusMessage() {
    createImage("alpha.jpg", Qt::red);
    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path())});

    QString const outputPath = m_tempDir->path() + QDir::separator() + "output";
    QVERIFY(QDir(outputPath).removeRecursively());

    QFile replacement(outputPath);
    QVERIFY(replacement.open(QIODevice::WriteOnly));
    replacement.write("not a directory");
    replacement.close();

    QTest::mouseClick(findButton("savePB"), Qt::LeftButton);

    QVERIFY(QFile::exists(m_tempDir->path() + QDir::separator() + "alpha.jpg"));
    QVERIFY(findStatusBar()->currentMessage().contains("Could not prepare 'output' directory"));
    QVERIFY(findStatusBar()->currentMessage().contains("non-directory"));
    QVERIFY(!findAction("Undo")->isEnabled());
    QVERIFY(!findAction("Redo")->isEnabled());
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_TrashFailure_ShowsStatusMessage() {
    createImage("alpha.jpg", Qt::red);
    sendDropWithUrls({QUrl::fromLocalFile(m_tempDir->path())});

    QString const trashPath = m_tempDir->path() + QDir::separator() + "trash";
    QVERIFY(QDir(trashPath).removeRecursively());

    QFile replacement(trashPath);
    QVERIFY(replacement.open(QIODevice::WriteOnly));
    replacement.write("not a directory");
    replacement.close();

    QTest::mouseClick(findButton("trashPB"), Qt::LeftButton);

    QVERIFY(QFile::exists(m_tempDir->path() + QDir::separator() + "alpha.jpg"));
    QVERIFY(findStatusBar()->currentMessage().contains("Could not prepare 'trash' directory"));
    QVERIFY(findStatusBar()->currentMessage().contains("non-directory"));
    QVERIFY(!findAction("Undo")->isEnabled());
    QVERIFY(!findAction("Redo")->isEnabled());
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_AboutAction_ShowsDialogAndStatus() {
    findThemeAction("dark")->trigger();
    QApplication::processEvents();

    findAction("About Cullendula")->trigger();
    QApplication::processEvents();

    QDialog* dialog = findOpenDialog();
    QVERIFY(dialog != nullptr);
    QCOMPARE(dialog->palette().color(QPalette::Window), qApp->palette().color(QPalette::Window));
    QVERIFY(qApp->styleSheet().contains("QMessageBox QLabel"));
    dialog->accept();
    QApplication::processEvents();

    QCOMPARE(findStatusBar()->currentMessage(), QString("Invoked Help|About"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_AboutQtAction_ShowsDialogAndStatus() {
    findThemeAction("dark")->trigger();
    QApplication::processEvents();

    findAction("About Qt")->trigger();
    QApplication::processEvents();

    QDialog* dialog = findOpenDialog();
    QVERIFY(dialog != nullptr);
    QCOMPARE(dialog->palette().color(QPalette::Window), qApp->palette().color(QPalette::Window));
    QVERIFY(qApp->styleSheet().contains("QMessageBox QLabel"));
    dialog->accept();
    QApplication::processEvents();

    QCOMPARE(findStatusBar()->currentMessage(), QString("Invoked Help|About Qt"));
}
