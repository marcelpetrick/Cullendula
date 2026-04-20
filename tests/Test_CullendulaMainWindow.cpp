//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#include "Test_CullendulaMainWindow.h"

#include <QtCore/QMimeData>
#include <QtCore/QRegularExpression>
#include <QtCore/QTimer>
#include <QtGui/QAction>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QImage>
#include <QtWidgets/QApplication>
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
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_InitialState() {
    QVERIFY(m_window->windowTitle().contains("v0.6.8"));
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
    QVERIFY(m_window->styleSheet().contains("#f6f3ee"));
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
    QVERIFY(m_window->styleSheet().contains("#0b0f14"));
    QVERIFY(m_window->styleSheet().contains("#74b9ff"));
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

    findAction("Redo")->trigger();
    QApplication::processEvents();
    QVERIFY(!QFile::exists(originalPath));
    QVERIFY(QFile::exists(movedPath));
    QVERIFY(findAction("Undo")->isEnabled());
    QVERIFY(!findAction("Redo")->isEnabled());
}

//----------------------------------------------------------------------------------

void Test_CullendulaMainWindow::slot_Test_AboutAction_ShowsDialogAndStatus() {
    QTimer::singleShot(0, []() {
        for (QWidget* widget : QApplication::topLevelWidgets()) {
            if (auto* messageBox = qobject_cast<QMessageBox*>(widget)) {
                messageBox->accept();
                return;
            }
        }
    });

    findAction("About Cullendula")->trigger();
    QApplication::processEvents();

    QCOMPARE(findStatusBar()->currentMessage(), QString("Invoked Help|About"));
}
