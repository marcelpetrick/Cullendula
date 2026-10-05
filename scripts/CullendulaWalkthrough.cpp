//----------------------------------------------------------------------------------
// description: Captures the real decorated Cullendula window for walkthrough media
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#include <QtCore/QDir>
#include <QtCore/QEventLoop>
#include <QtCore/QMimeData>
#include <QtCore/QResource>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtGui/QAction>
#include <QtGui/QCursor>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <stdexcept>

#include "CullendulaAppBootstrap.h"
#include "CullendulaMainWindow.h"

namespace {
void waitForUi(int delayMs) {
    QEventLoop eventLoop;
    QTimer::singleShot(delayMs, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
}

QAction* findAction(QObject& root, QString const& objectName) {
    QAction* action = root.findChild<QAction*>(objectName);
    if (action == nullptr) {
        throw std::runtime_error(QString("Could not find action %1").arg(objectName).toStdString());
    }
    return action;
}

QAction* findAction(QObject& root, QKeySequence const& shortcut) {
    for (QAction* action : root.findChildren<QAction*>()) {
        if (action->shortcut() == shortcut) {
            return action;
        }
    }
    throw std::runtime_error(QString("Could not find action with shortcut %1").arg(shortcut.toString()).toStdString());
}

QPushButton* findButton(QObject& root, QString const& objectName) {
    QPushButton* button = root.findChild<QPushButton*>(objectName);
    if (button == nullptr) {
        throw std::runtime_error(QString("Could not find button %1").arg(objectName).toStdString());
    }
    return button;
}

QMenu* findContainingMenu(QObject& root, QAction* action) {
    for (QMenu* menu : root.findChildren<QMenu*>()) {
        if (menu->actions().contains(action)) {
            return menu;
        }
    }
    throw std::runtime_error(QString("Could not find menu for action %1").arg(action->text()).toStdString());
}

void closeMenus(QObject& root) {
    for (QMenu* menu : root.findChildren<QMenu*>()) {
        menu->close();
    }
}

void focusMainWindow(CullendulaMainWindow& window) {
    closeMenus(window);
    window.raise();
    window.activateWindow();
    QApplication::processEvents();
    QCursor::setPos(window.mapToGlobal(window.rect().center()));
}

void showMenuForAction(CullendulaMainWindow& window, QAction* action) {
    focusMainWindow(window);
    QMenu* menu = findContainingMenu(window, action);
    menu->popup(window.mapToGlobal(QPoint(42, 58)));
    QApplication::processEvents();
    QCursor::setPos(menu->mapToGlobal(menu->rect().center()));
}

void triggerAction(CullendulaMainWindow& window, QAction* action) {
    focusMainWindow(window);
    action->trigger();
    QApplication::processEvents();
}

void clickButton(CullendulaMainWindow& window, QString const& objectName) {
    focusMainWindow(window);
    findButton(window, objectName)->animateClick();
    waitForUi(180);
}

void dropDirectory(CullendulaMainWindow& window, QString const& path) {
    QMimeData mimeData;
    mimeData.setUrls({QUrl::fromLocalFile(path)});

    QPoint const center = window.rect().center();
    QDragEnterEvent dragEnter(QPointF(center), Qt::CopyAction, &mimeData, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(&window, &dragEnter);

    QDropEvent drop(QPointF(center), Qt::CopyAction, &mimeData, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(&window, &drop);
    QApplication::processEvents();
}

QMessageBox* findInformationDialog(CullendulaMainWindow& window) {
    for (QMessageBox* messageBox : window.findChildren<QMessageBox*>()) {
        if (messageBox->isVisible()) {
            return messageBox;
        }
    }
    return nullptr;
}

void focusInformationDialog(CullendulaMainWindow& window) {
    QApplication::processEvents();
    waitForUi(220);
    QMessageBox* messageBox = findInformationDialog(window);
    if (messageBox == nullptr) {
        throw std::runtime_error("Could not find the visible information dialog");
    }
    messageBox->raise();
    messageBox->activateWindow();
    QCursor::setPos(messageBox->mapToGlobal(messageBox->rect().center()));
}

void closeInformationDialogs(CullendulaMainWindow& window) {
    for (QMessageBox* messageBox : window.findChildren<QMessageBox*>()) {
        messageBox->accept();
    }
    QApplication::processEvents();
}

class WalkthroughPacer {
   public:
    WalkthroughPacer(CullendulaMainWindow& window, QString const& frameDirectory) : m_window(window), m_frameDirectory(frameDirectory) {}

    void mainWindow(QString const& label) {
        focusMainWindow(m_window);
        capture(label);
    }

    void currentWindow(QString const& label) { capture(label); }

   private:
    void capture(QString const& label) {
        waitForUi(180);

        QPixmap frame = m_window.grab();
        QPainter painter(&frame);
        for (QWidget* widget : QApplication::topLevelWidgets()) {
            if (widget == &m_window || !widget->isVisible()) {
                continue;
            }
            QPoint const position = m_window.mapFromGlobal(widget->geometry().topLeft());
            painter.drawPixmap(position, widget->grab());
        }
        painter.end();

        QString const path = QStringLiteral("%1/%2-%3.png").arg(m_frameDirectory).arg(m_frameIndex++, 3, 10, QLatin1Char('0')).arg(label);
        if (!frame.save(path)) {
            throw std::runtime_error(QString("Could not save walkthrough frame %1").arg(path).toStdString());
        }
    }

    CullendulaMainWindow& m_window;
    QString m_frameDirectory;
    int m_frameIndex = 0;
};

struct WalkthroughActions {
    QAction* light;
    QAction* dark;
    QAction* purple;
    QAction* german;
    QAction* chinese;
    QAction* english;
    QAction* firstExtension;
    QAction* undo;
    QAction* redo;
    QAction* about;
    QAction* aboutQt;
};

WalkthroughActions findWalkthroughActions(CullendulaMainWindow& window) {
    return {findAction(window, QStringLiteral("themeAction_light")),    findAction(window, QStringLiteral("themeAction_dark")),
            findAction(window, QStringLiteral("themeAction_purple")),   findAction(window, QStringLiteral("languageAction_de")),
            findAction(window, QStringLiteral("languageAction_zh_CN")), findAction(window, QStringLiteral("languageAction_en")),
            findAction(window, QStringLiteral("extensionAction_jpg")),  findAction(window, QKeySequence(Qt::CTRL | Qt::Key_Y)),
            findAction(window, QKeySequence(Qt::CTRL | Qt::Key_Z)),     findAction(window, QKeySequence(Qt::CTRL | Qt::Key_A)),
            findAction(window, QKeySequence(Qt::CTRL | Qt::Key_Q))};
}

void captureReadmeWalkthrough(CullendulaMainWindow& window, QString const& demoDirectory, QString const& frameDirectory) {
    WalkthroughActions const actions = findWalkthroughActions(window);
    WalkthroughPacer capture(window, frameDirectory);

    capture.mainWindow(QStringLiteral("intro"));
    dropDirectory(window, demoDirectory);
    capture.mainWindow(QStringLiteral("session-loaded"));
    clickButton(window, QStringLiteral("rightPB"));
    capture.mainWindow(QStringLiteral("browse-next"));
    clickButton(window, QStringLiteral("rightPB"));
    capture.mainWindow(QStringLiteral("browse-next"));
    clickButton(window, QStringLiteral("leftPB"));
    clickButton(window, QStringLiteral("savePB"));
    capture.mainWindow(QStringLiteral("keep"));
    clickButton(window, QStringLiteral("trashPB"));
    capture.mainWindow(QStringLiteral("reject"));
    triggerAction(window, actions.undo);
    capture.mainWindow(QStringLiteral("undo"));
    triggerAction(window, actions.redo);
    capture.mainWindow(QStringLiteral("redo"));
    triggerAction(window, actions.undo);
    showMenuForAction(window, actions.dark);
    capture.currentWindow(QStringLiteral("themes-menu"));
    triggerAction(window, actions.dark);
    capture.mainWindow(QStringLiteral("theme-dark"));
    showMenuForAction(window, actions.purple);
    capture.currentWindow(QStringLiteral("themes-menu"));
    triggerAction(window, actions.purple);
    capture.mainWindow(QStringLiteral("theme-purple"));
    triggerAction(window, actions.light);
    showMenuForAction(window, actions.german);
    capture.currentWindow(QStringLiteral("languages-menu"));
    triggerAction(window, actions.german);
    capture.mainWindow(QStringLiteral("language-german"));
    showMenuForAction(window, actions.chinese);
    capture.currentWindow(QStringLiteral("languages-menu"));
    triggerAction(window, actions.chinese);
    capture.mainWindow(QStringLiteral("language-chinese"));
    triggerAction(window, actions.english);
    showMenuForAction(window, actions.firstExtension);
    capture.currentWindow(QStringLiteral("formats"));
    showMenuForAction(window, actions.about);
    capture.currentWindow(QStringLiteral("help-menu"));
    triggerAction(window, actions.about);
    focusInformationDialog(window);
    capture.currentWindow(QStringLiteral("about"));
    closeInformationDialogs(window);
    triggerAction(window, actions.aboutQt);
    focusInformationDialog(window);
    capture.currentWindow(QStringLiteral("about-qt"));
    closeInformationDialogs(window);
    capture.mainWindow(QStringLiteral("finished"));
}

void captureSocialWalkthrough(CullendulaMainWindow& window, QString const& demoDirectory, QString const& frameDirectory) {
    WalkthroughActions const actions = findWalkthroughActions(window);
    WalkthroughPacer capture(window, frameDirectory);

    capture.mainWindow(QStringLiteral("intro"));
    dropDirectory(window, demoDirectory);
    capture.mainWindow(QStringLiteral("session-loaded"));
    clickButton(window, QStringLiteral("rightPB"));
    capture.mainWindow(QStringLiteral("browse-next"));
    showMenuForAction(window, actions.purple);
    capture.currentWindow(QStringLiteral("themes-menu"));
    triggerAction(window, actions.purple);
    capture.mainWindow(QStringLiteral("theme-purple"));
    triggerAction(window, actions.light);
    showMenuForAction(window, actions.german);
    capture.currentWindow(QStringLiteral("languages-menu"));
    triggerAction(window, actions.german);
    capture.mainWindow(QStringLiteral("language-german"));
    triggerAction(window, actions.english);
    clickButton(window, QStringLiteral("savePB"));
    capture.mainWindow(QStringLiteral("keep"));
    clickButton(window, QStringLiteral("trashPB"));
    capture.mainWindow(QStringLiteral("reject"));
    triggerAction(window, actions.undo);
    capture.mainWindow(QStringLiteral("undo"));
    triggerAction(window, actions.redo);
    capture.mainWindow(QStringLiteral("redo"));
    triggerAction(window, actions.undo);
    showMenuForAction(window, actions.firstExtension);
    capture.currentWindow(QStringLiteral("formats"));
    showMenuForAction(window, actions.dark);
    capture.currentWindow(QStringLiteral("themes-menu"));
    triggerAction(window, actions.dark);
    capture.mainWindow(QStringLiteral("theme-dark"));
    triggerAction(window, actions.light);
    showMenuForAction(window, actions.about);
    capture.currentWindow(QStringLiteral("help-menu"));
    triggerAction(window, actions.about);
    focusInformationDialog(window);
    capture.currentWindow(QStringLiteral("about"));
    closeInformationDialogs(window);
    triggerAction(window, actions.aboutQt);
    focusInformationDialog(window);
    capture.currentWindow(QStringLiteral("about-qt"));
    closeInformationDialogs(window);
    capture.mainWindow(QStringLiteral("finished"));
}
}  // namespace

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    if (argc != 5) {
        qCritical("Usage: CullendulaWalkthrough <demo-directory> <translation-resource.rcc> <readme|social|shell> <frame-directory>");
        return 2;
    }

    QString const demoDirectory = QDir(QString::fromLocal8Bit(argv[1])).absolutePath();
    QString const translationResource = QDir::cleanPath(QString::fromLocal8Bit(argv[2]));
    QString const variant = QString::fromLocal8Bit(argv[3]);
    QString const frameDirectory = QDir::cleanPath(QString::fromLocal8Bit(argv[4]));
    if (!QResource::registerResource(translationResource)) {
        qCritical("Could not register translation resource: %s", qPrintable(translationResource));
        return 3;
    }

    CullendulaAppBootstrap::applyApplicationIcon();
    CullendulaMainWindow window;
    window.setFixedSize(800, 600);
    window.move(140, 140);
    window.show();
    QApplication::processEvents();

    QTimer::singleShot(500, &app, [&]() {
        try {
            if (variant == QStringLiteral("readme")) {
                captureReadmeWalkthrough(window, demoDirectory, frameDirectory);
            } else if (variant == QStringLiteral("social")) {
                captureSocialWalkthrough(window, demoDirectory, frameDirectory);
            } else if (variant == QStringLiteral("shell")) {
                waitForUi(10000);
            } else {
                qCritical("Unknown walkthrough variant: %s", qPrintable(variant));
                app.exit(4);
                return;
            }
        } catch (std::exception const& error) {
            qCritical("Could not capture walkthrough: %s", error.what());
            app.exit(5);
            return;
        }
        app.quit();
    });

    return QApplication::exec();
}
