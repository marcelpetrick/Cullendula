//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

// own includes
#include "CullendulaMainWindow.h"

#include "ui_CullendulaMainWindow.h"

// Qt includes
#include <QtCore/QDebug>  //todom maybe remove
#include <QtCore/QEvent>
#include <QtCore/QLibraryInfo>
#include <QtCore/QMimeData>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QColor>
#include <QtGui/QDropEvent>
#include <QtGui/QPalette>
#include <QtGui/QPixmap>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStyleFactory>

//----------------------------------------------------------------------------

// for constants
namespace {
QString applicationVersionSuffix() { return QStringLiteral(" - v" CULLENDULA_PROJECT_VERSION); }

//! Determines how long the status message is visible. After timer runs out, it is removed.
unsigned int const c_StatusBarDelay = 5000;

//! prevent pumping window because of scaling. Describes the expected size of the frame.
int const c_extraPixelsBecauseOfFraming = 2;

struct ThemeDefinition {
    QColor windowColor;
    QColor surfaceColor;
    QColor menuSurfaceColor;
    QColor textColor;
    QColor mutedTextColor;
    QColor borderColor;
    QColor buttonColor;
    QColor buttonHoverColor;
    QColor buttonPressedColor;
    QColor disabledButtonColor;
    QColor disabledBorderColor;
    QColor accentColor;
    QColor accentHoverColor;
    QColor accentPressedColor;
    QColor highlightColor;
    QColor tooltipBaseColor;
    QColor tooltipTextColor;
};

ThemeDefinition getLightThemeDefinition() {
    return {QColor("#f6f3ee"), QColor("#fffaf2"), QColor("#ece6dc"), QColor("#1f252d"), QColor("#7a7f86"), QColor("#c9bba8"),
            QColor("#efe2cc"), QColor("#e3d1b5"), QColor("#d3bd9d"), QColor("#ddd7ce"), QColor("#c8c0b5"), QColor("#b79d7f"),
            QColor("#d7c9b4"), QColor("#c8b08b"), QColor("#d7c9b4"), QColor("#fff6df"), QColor("#1f252d")};
}

ThemeDefinition getDarkThemeDefinition() {
    return {QColor("#0b0f14"), QColor("#131c26"), QColor("#111821"), QColor("#f4f7fb"), QColor("#7a8795"), QColor("#3e5875"),
            QColor("#16324b"), QColor("#1d4668"), QColor("#24567f"), QColor("#1b2128"), QColor("#39424d"), QColor("#79c0ff"),
            QColor("#24415f"), QColor("#2f5d87"), QColor("#24415f"), QColor("#182634"), QColor("#f4f7fb")};
}

ThemeDefinition getPurpleThemeDefinition() {
    return {QColor("#110d1b"), QColor("#171127"), QColor("#1d1630"), QColor("#f0ecff"), QColor("#9d93ba"), QColor("#53436e"),
            QColor("#372454"), QColor("#4a2f6e"), QColor("#5a3b84"), QColor("#221b31"), QColor("#3e3353"), QColor("#63d5f7"),
            QColor("#6fe3ff"), QColor("#3cc7ef"), QColor("#2a3d5e"), QColor("#171127"), QColor("#dbfaff")};
}

QPalette createThemePalette(ThemeDefinition const& theme) {
    QPalette palette;

    palette.setColor(QPalette::Window, theme.windowColor);
    palette.setColor(QPalette::WindowText, theme.textColor);
    palette.setColor(QPalette::Base, theme.surfaceColor);
    palette.setColor(QPalette::AlternateBase, theme.menuSurfaceColor);
    palette.setColor(QPalette::ToolTipBase, theme.tooltipBaseColor);
    palette.setColor(QPalette::ToolTipText, theme.tooltipTextColor);
    palette.setColor(QPalette::Text, theme.textColor);
    palette.setColor(QPalette::Button, theme.buttonColor);
    palette.setColor(QPalette::ButtonText, theme.textColor);
    palette.setColor(QPalette::BrightText, Qt::white);
    palette.setColor(QPalette::Link, theme.accentColor);
    palette.setColor(QPalette::Highlight, theme.highlightColor);
    palette.setColor(QPalette::HighlightedText, theme.textColor);
    palette.setColor(QPalette::PlaceholderText, theme.mutedTextColor);

    palette.setColor(QPalette::Disabled, QPalette::WindowText, theme.mutedTextColor);
    palette.setColor(QPalette::Disabled, QPalette::Text, theme.mutedTextColor);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, theme.mutedTextColor);
    palette.setColor(QPalette::Disabled, QPalette::Button, theme.disabledButtonColor);
    palette.setColor(QPalette::Disabled, QPalette::Highlight, theme.disabledBorderColor);

    return palette;
}

QString getStandardThemeStyleSheet(ThemeDefinition const& theme) {
    return QStringLiteral(
               "QMainWindow, QDialog, QMessageBox {"
               "    background-color: %1;"
               "    color: %2;"
               "}"
               "QWidget#centralWidget {"
               "    background-color: %1;"
               "    color: %2;"
               "}"
               "QMenuBar {"
               "    background-color: %3;"
               "    color: %2;"
               "}"
               "QMenuBar::item:selected {"
               "    background-color: %4;"
               "}"
               "QMenu {"
               "    background-color: %5;"
               "    color: %2;"
               "    border: 1px solid %6;"
               "}"
               "QMenu::item:selected {"
               "    background-color: %4;"
               "}"
               "QToolTip {"
               "    background-color: %5;"
               "    color: %7;"
               "    border: 1px solid %6;"
               "}"
               "QLabel#centerLabel {"
               "    background-color: %5;"
               "    color: %2;"
               "    border: 2px solid %8;"
               "    padding: 18px;"
               "}"
               "QLabel {"
               "    color: %2;"
               "}"
               "QMessageBox QLabel {"
               "    color: %2;"
               "}"
               "QPushButton {"
               "    background-color: %9;"
               "    color: %2;"
               "    border: 1px solid %8;"
               "    border-radius: 6px;"
               "    padding: 8px 14px;"
               "}"
               "QPushButton:hover:!disabled {"
               "    background-color: %10;"
               "}"
               "QPushButton:pressed:!disabled {"
               "    background-color: %11;"
               "}"
               "QPushButton:disabled {"
               "    background-color: %12;"
               "    color: %13;"
               "    border-color: %14;"
               "}"
               "QStatusBar {"
               "    background-color: %3;"
               "    color: %2;"
               "}"
               "QStatusBar::item {"
               "    border: none;"
               "}")
        .arg(theme.windowColor.name(), theme.textColor.name(), theme.menuSurfaceColor.name(), theme.highlightColor.name(), theme.surfaceColor.name(),
             theme.borderColor.name(), theme.tooltipTextColor.name(), theme.accentColor.name(), theme.buttonColor.name(), theme.buttonHoverColor.name(),
             theme.buttonPressedColor.name(), theme.disabledButtonColor.name(), theme.mutedTextColor.name(), theme.disabledBorderColor.name());
}

QString getPurpleThemeStyleSheet(ThemeDefinition const& theme) {
    return QStringLiteral(
               "QMainWindow, QDialog, QMessageBox {"
               "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 %1, stop:1 %15);"
               "    color: %2;"
               "}"
               "QWidget#centralWidget {"
               "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 %1, stop:1 %15);"
               "    color: %2;"
               "}"
               "QMenuBar {"
               "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 %3, stop:1 %16);"
               "    color: %2;"
               "}"
               "QMenuBar::item:selected {"
               "    background-color: %4;"
               "}"
               "QMenu {"
               "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 %5, stop:1 %15);"
               "    color: %2;"
               "    border: 1px solid %6;"
               "}"
               "QMenu::item:selected {"
               "    background-color: %4;"
               "}"
               "QToolTip {"
               "    background-color: %5;"
               "    color: %7;"
               "    border: 1px solid %6;"
               "}"
               "QLabel#centerLabel {"
               "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 %5, stop:1 %15);"
               "    color: %2;"
               "    border: 2px solid %8;"
               "    padding: 18px;"
               "}"
               "QLabel {"
               "    color: %2;"
               "}"
               "QMessageBox QLabel {"
               "    color: %2;"
               "}"
               "QPushButton {"
               "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 %9, stop:1 %17);"
               "    color: %2;"
               "    border: 1px solid %8;"
               "    border-radius: 6px;"
               "    padding: 8px 14px;"
               "}"
               "QPushButton:hover:!disabled {"
               "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 %10, stop:1 %8);"
               "}"
               "QPushButton:pressed:!disabled {"
               "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 %11, stop:1 %14);"
               "}"
               "QPushButton:disabled {"
               "    background-color: %12;"
               "    color: %13;"
               "    border-color: %14;"
               "}"
               "QStatusBar {"
               "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 %3, stop:1 %16);"
               "    color: %2;"
               "}"
               "QStatusBar::item {"
               "    border: none;"
               "}")
        .arg(theme.windowColor.name(), theme.textColor.name(), theme.menuSurfaceColor.name(), theme.highlightColor.name(), theme.surfaceColor.name(),
             theme.borderColor.name(), theme.tooltipTextColor.name(), theme.accentColor.name(), theme.buttonColor.name(), theme.buttonHoverColor.name(),
             theme.buttonPressedColor.name(), theme.disabledButtonColor.name(), theme.mutedTextColor.name(), theme.disabledBorderColor.name(),
             theme.tooltipBaseColor.name(), theme.highlightColor.darker(155).name(), theme.buttonPressedColor.darker(135).name());
}
}  // namespace

//----------------------------------------------------------------------------

CullendulaMainWindow::CullendulaMainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::CullendulaMainWindow) {
    setAcceptDrops(true);
    ui->setupUi(this);

    // make the current version information visible to the user - as long as no menu with help&stuff exists
    // disable the buttons immediately; until the directory is set
    activateButtons(false);

    // connects for the usable push-buttons (and their hot keys)
    connect(ui->leftPB, &QPushButton::clicked, this, &CullendulaMainWindow::slotButtonLeftTriggered);
    connect(ui->rightPB, &QPushButton::clicked, this, &CullendulaMainWindow::slotButtonRightTriggered);
    connect(ui->savePB, &QPushButton::clicked, this, &CullendulaMainWindow::slotButtonSaveTriggered);
    connect(ui->trashPB, &QPushButton::clicked, this, &CullendulaMainWindow::slotButtonTrashTriggered);

    // necessary! even if the QLabel itself accepts already drops
    setAcceptDrops(true);

    // create the menu
    createActions();
    createMenus();
    retranslateStaticTexts();
    applyTheme(ThemeMode::Light);
    applyLanguage(CullendulaAppBootstrap::getApplicationLanguage());
    syncAllowedExtensionsToFileSystemHandler();

    // set undo/redo correctly
    updateUndoRedoButtonStatus();

    //: Status bar message shown once after the main window is fully initialized.
    printStatus(tr("system is up and running :)"));
}

//----------------------------------------------------------------------------

CullendulaMainWindow::~CullendulaMainWindow() { delete ui; }

//----------------------------------------------------------------------------

CullendulaMainWindow::ThemeMode CullendulaMainWindow::getThemeMode() const { return m_themeMode; }

//----------------------------------------------------------------------------

void CullendulaMainWindow::changeEvent(QEvent* event) {
    QMainWindow::changeEvent(event);

    if (event != nullptr && event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        retranslateStaticTexts();
        refreshLabel();
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::dragEnterEvent(QDragEnterEvent* event) {
    if (event != nullptr && event->mimeData() != nullptr && event->mimeData()->hasUrls() && !event->mimeData()->urls().isEmpty()) {
        event->acceptProposedAction();
        //: Status bar prompt during drag-and-drop after the payload was recognized as file-system URLs.
        printStatus(tr("drop current load and let's see what you dragged?"));
        return;
    }

    if (event != nullptr) {
        event->ignore();
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::dropEvent(QDropEvent* event) {
    const QMimeData* mimeData = event->mimeData();

    // check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls()) {
        const QList<QUrl> urlList = mimeData->urls();

        // extract the local paths of the files: print all of them; can be removed laters
        qDebug() << "new drop:";
        for (auto const& url : urlList) {
            qDebug() << "\tdropped: " << url;
        }

        // just use the very first one ..
        if (!urlList.isEmpty()) {
            bool const success = m_fileSystemHandler.setWorkingPath(urlList.first().path());
            refreshLabel();
            updateUndoRedoButtonStatus();
            if (!success) {
                QString const errorMessage = m_fileSystemHandler.getLastErrorMessage();
                if (!errorMessage.isEmpty()) {
                    printStatus(errorMessage);
                } else {
                    qDebug() << "\tNo matching image files found for the current filter";
                }
            }
        }
    } else {
        //: Status bar error after a drop payload was rejected because it did not contain usable local file URLs.
        printStatus(tr("The load was not usable! :("));
    }

    event->acceptProposedAction();
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::resizeEvent(QResizeEvent* event) {
    qDebug() << "CullendulaMainWindow::resizeEvent()";

    QMainWindow::resizeEvent(event);

    // reload the current picture (or text)
    refreshLabel();
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::slotButtonLeftTriggered() {
    qDebug() << "CullendulaMainWindow::slotButtonLeftTriggered()";

    bool const success = m_fileSystemHandler.switchCurrentPositionToTheLeft();
    // check if that was successful and then refresh
    if (success) {
        refreshLabel();
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::slotButtonRightTriggered() {
    qDebug() << "CullendulaMainWindow::slotButtonRightTriggered()";

    bool const success = m_fileSystemHandler.switchCurrentPositionToTheRight();
    // check if that was successful and then refresh
    if (success) {
        refreshLabel();
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::slotButtonSaveTriggered() {
    qDebug() << "CullendulaMainWindow::slotButtonSaveTriggered()";

    // move the current file to the output-folder
    bool const success = m_fileSystemHandler.saveCurrentFile();
    if (success) {
        refreshLabel();
        // set undo/redo correctly
        updateUndoRedoButtonStatus();
    } else {
        QString const errorMessage = m_fileSystemHandler.getLastErrorMessage();
        //: Fallback status bar error after moving the current image to the "output" folder failed without a more specific message.
        printStatus(errorMessage.isEmpty() ? tr("Could not save the current file.") : errorMessage);
        qDebug() << "\tERROR: moving file did not succeed";
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::slotButtonTrashTriggered() {
    qDebug() << "CullendulaMainWindow::slotButtonTrashTriggered()";

    // move the current file to the output-folder
    bool const success = m_fileSystemHandler.trashCurrentFile();
    if (success) {
        refreshLabel();
        // set undo/redo correctly
        updateUndoRedoButtonStatus();
    } else {
        QString const errorMessage = m_fileSystemHandler.getLastErrorMessage();
        //: Fallback status bar error after moving the current image to the "trash" folder failed without a more specific message.
        printStatus(errorMessage.isEmpty() ? tr("Could not trash the current file.") : errorMessage);
        qDebug() << "\tERROR: moving file did not succeed";
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::loadAndCachePhoto(QString const& path) {
    if (m_cachedImagePath == path && !m_cachedPhoto.isNull()) {
        return;
    }

    qDebug() << "CullendulaMainWindow::loadAndCachePhoto(): path=" << path;

    QPixmap const pixmap(path);
    if (pixmap.isNull()) {
        clearCachedPhoto();
        return;
    }

    m_cachedImagePath = path;
    m_cachedPhoto = pixmap;
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::showCachedPhoto() {
    if (m_cachedPhoto.isNull()) {
        ui->centerLabel->clear();
        return;
    }

    // scale while keeping the aspect ratio
    int width = ui->centerLabel->width() - c_extraPixelsBecauseOfFraming;
    int height = ui->centerLabel->height() - c_extraPixelsBecauseOfFraming;
    qDebug() << "label-size:" << width << "*" << height;

    // prevent upscaling of smaller photos
    if (m_cachedPhoto.width() < width) {
        width = m_cachedPhoto.width();
    }

    if (m_cachedPhoto.height() < height) {
        height = m_cachedPhoto.height();
    }

    // assert that both values are positive
    width = std::max(0, width);
    height = std::max(0, height);

    // set a scaled pixmap keeping its aspect ratio
    ui->centerLabel->setPixmap(m_cachedPhoto.scaled(width, height, Qt::KeepAspectRatio));
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::clearCachedPhoto() {
    m_cachedImagePath.clear();
    m_cachedPhoto = QPixmap();
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::refreshLabel() {
    qDebug() << "CullendulaMainWindow::refreshLabel():";

    QString const path = m_fileSystemHandler.getCurrentImagePath();
    if (path.isEmpty())  // just the case if no valid images found
    {
        clearCachedPhoto();
        //: Center label placeholder when no further images match the current extension filter in the active directory.
        ui->centerLabel->setText(
            tr("no more valid images found: work maybe finished? :)\n"
               "drag&drop the next folder or files if you want!"));

        activateButtons(false);

        //: Status bar message when the current directory no longer contains any matching images to show.
        printStatus(tr("no more files"));
    } else {
        if (QFile::exists(path)) {
            loadAndCachePhoto(path);
            if (m_cachedPhoto.isNull()) {
                clearCachedPhoto();
                //: Error shown both in the center label and the status bar when Qt cannot render the current image preview.
                ui->centerLabel->setText(tr("could not load the current image preview"));
                activateButtons(false);
                printStatus(tr("could not load the current image preview"));
                return;
            }

            // scale the cached file to the current label geometry
            showCachedPhoto();

            activateButtons(true);

            // print the current file-path as user-notification
            QString const message = m_fileSystemHandler.getCurrentStatus() + ": " + path;
            printStatus(message);
        } else {
            clearCachedPhoto();
            qDebug() << "CullendulaMainWindow::refreshLabel(): given path did not exist: " << path;
        }
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::activateButtons(const bool active) const {
    // en-/disable all four buttons
    ui->leftPB->setEnabled(active);
    ui->rightPB->setEnabled(active);
    ui->savePB->setEnabled(active);
    ui->trashPB->setEnabled(active);
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::printStatus(const QString& message) const {
    // messages shall disappear after five seconds
    ui->statusBar->showMessage(message, c_StatusBarDelay);
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::createActions() {
    QStringList const suggestedExtensions = CullendulaFileSystemHandler::getSuggestedImageExtensions();
    for (QString const& extension : suggestedExtensions) {
        QAction* extensionAction = new QAction(extension.toUpper(), this);
        extensionAction->setCheckable(true);
        extensionAction->setChecked(true);
        extensionAction->setObjectName("extensionAction_" + extension);
        //: Tooltip for a checkable menu entry that enables a filename extension such as jpg, png, or webp for future directory scans.
        extensionAction->setStatusTip(tr("Enable loading of *.%1 files when opening the next directory").arg(extension));
        connect(extensionAction, &QAction::toggled, this, [this]() { syncAllowedExtensionsToFileSystemHandler(); });
        m_extensionActions.insert(extension, extensionAction);
    }

    m_themeActionGroup = new QActionGroup(this);
    m_themeActionGroup->setExclusive(true);

    //: Menu label for the light visual theme.
    m_lightThemeAction = new QAction(tr("Light"), this);
    m_lightThemeAction->setCheckable(true);
    m_lightThemeAction->setObjectName("themeAction_light");
    //: Tooltip for switching the whole application to the light theme.
    m_lightThemeAction->setStatusTip(tr("Use the light application theme"));
    connect(m_lightThemeAction, &QAction::triggered, this, [this]() { applyTheme(ThemeMode::Light); });
    m_themeActionGroup->addAction(m_lightThemeAction);

    //: Menu label for the dark visual theme.
    m_darkThemeAction = new QAction(tr("Dark"), this);
    m_darkThemeAction->setCheckable(true);
    m_darkThemeAction->setObjectName("themeAction_dark");
    //: Tooltip for switching the whole application to the dark theme.
    m_darkThemeAction->setStatusTip(tr("Use the high-contrast dark application theme"));
    connect(m_darkThemeAction, &QAction::triggered, this, [this]() { applyTheme(ThemeMode::Dark); });
    m_themeActionGroup->addAction(m_darkThemeAction);

    //: Menu label for the gloomy purple visual theme.
    m_purpleThemeAction = new QAction(tr("Purple"), this);
    m_purpleThemeAction->setCheckable(true);
    m_purpleThemeAction->setObjectName("themeAction_purple");
    //: Tooltip for switching the whole application to the gloomy purple-and-cyan theme.
    m_purpleThemeAction->setStatusTip(tr("Use the gloomy purple application theme"));
    connect(m_purpleThemeAction, &QAction::triggered, this, [this]() { applyTheme(ThemeMode::Purple); });
    m_themeActionGroup->addAction(m_purpleThemeAction);

    m_languageActionGroup = new QActionGroup(this);
    m_languageActionGroup->setExclusive(true);

    //: Language menu entry naming the English user-interface language.
    m_englishLanguageAction = new QAction(tr("English"), this);
    m_englishLanguageAction->setCheckable(true);
    m_englishLanguageAction->setObjectName("languageAction_en");
    connect(m_englishLanguageAction, &QAction::triggered, this, [this]() { applyLanguage(CullendulaAppBootstrap::UiLanguage::English); });
    m_languageActionGroup->addAction(m_englishLanguageAction);

    //: Language menu entry naming the German user-interface language in German.
    m_germanLanguageAction = new QAction(tr("Deutsch"), this);
    m_germanLanguageAction->setCheckable(true);
    m_germanLanguageAction->setObjectName("languageAction_de");
    connect(m_germanLanguageAction, &QAction::triggered, this, [this]() { applyLanguage(CullendulaAppBootstrap::UiLanguage::German); });
    m_languageActionGroup->addAction(m_germanLanguageAction);

    //: Language menu entry naming the Croatian user-interface language in Croatian.
    m_croatianLanguageAction = new QAction(tr("Hrvatski"), this);
    m_croatianLanguageAction->setCheckable(true);
    m_croatianLanguageAction->setObjectName("languageAction_hr");
    connect(m_croatianLanguageAction, &QAction::triggered, this, [this]() { applyLanguage(CullendulaAppBootstrap::UiLanguage::Croatian); });
    m_languageActionGroup->addAction(m_croatianLanguageAction);

    //: Language menu entry naming the Chinese user-interface language in Chinese.
    m_chineseLanguageAction = new QAction(tr("中文"), this);
    m_chineseLanguageAction->setCheckable(true);
    m_chineseLanguageAction->setObjectName("languageAction_zh_CN");
    connect(m_chineseLanguageAction, &QAction::triggered, this, [this]() { applyLanguage(CullendulaAppBootstrap::UiLanguage::Chinese); });
    m_languageActionGroup->addAction(m_chineseLanguageAction);

    // edit menu
    //: Edit menu action label that reverses the most recent file move.
    m_undoAction = new QAction(tr("Undo"), this);
    //: Tooltip for the Undo action that moves the previously moved image back to its original location.
    m_undoAction->setStatusTip(tr("Revert the last file-move-operation"));
    m_undoAction->setShortcut(Qt::CTRL | Qt::Key_Y);
    connect(m_undoAction, &QAction::triggered, this, [=]() {
        qDebug("pressed Undo");
        if (m_fileSystemHandler.undo()) {
            refreshLabel();
        } else {
            QString const errorMessage = m_fileSystemHandler.getLastErrorMessage();
            //: Fallback status bar error after an undo request failed without a more specific message.
            printStatus(errorMessage.isEmpty() ? tr("Could not undo the last file move.") : errorMessage);
        }
        updateUndoRedoButtonStatus();
    });

    //: Edit menu action label that reapplies the most recently undone file move.
    m_redoQtAction = new QAction(tr("Redo"), this);
    //: Tooltip for the Redo action. "undo undo" here means reapplied after an Undo.
    m_redoQtAction->setStatusTip(tr("Redo the last file-move-operation (means: undo undo)"));
    m_redoQtAction->setShortcut(Qt::CTRL | Qt::Key_Z);
    connect(m_redoQtAction, &QAction::triggered, this, [=]() {
        qDebug("pressed Redo");
        if (m_fileSystemHandler.redo()) {
            refreshLabel();
        } else {
            QString const errorMessage = m_fileSystemHandler.getLastErrorMessage();
            //: Fallback status bar error after a redo request failed without a more specific message.
            printStatus(errorMessage.isEmpty() ? tr("Could not redo the last file move.") : errorMessage);
        }
        updateUndoRedoButtonStatus();
    });

    // help menu
    //: Help menu action label that opens the application's About dialog.
    m_aboutAction = new QAction(tr("About Cullendula"), this);
    //: Tooltip for opening the application's About dialog.
    m_aboutAction->setStatusTip(tr("Show the application's About box"));
    m_aboutAction->setShortcut(Qt::CTRL | Qt::Key_A);
    connect(m_aboutAction, &QAction::triggered, this, &CullendulaMainWindow::about);

    //: Help menu action label that opens Qt's built-in About dialog.
    m_aboutQtAction = new QAction(tr("About Qt"), this);
    //: Tooltip for opening Qt's built-in About dialog.
    m_aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    m_aboutQtAction->setShortcut(Qt::CTRL | Qt::Key_Q);
    connect(m_aboutQtAction, &QAction::triggered, this, &CullendulaMainWindow::aboutQt);
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::createMenus() {
    // main menu
    m_mainMenu = menuBar()->addMenu(QString());
    m_extensionsMenu = m_mainMenu->addMenu(QString());
    for (QAction* extensionAction : m_extensionActions) {
        m_extensionsMenu->addAction(extensionAction);
    }
    m_styleMenu = m_mainMenu->addMenu(QString());
    m_styleMenu->addAction(m_lightThemeAction);
    m_styleMenu->addAction(m_darkThemeAction);
    m_styleMenu->addAction(m_purpleThemeAction);
    m_languageMenu = m_mainMenu->addMenu(QString());
    m_languageMenu->addAction(m_englishLanguageAction);
    m_languageMenu->addAction(m_germanLanguageAction);
    m_languageMenu->addAction(m_croatianLanguageAction);
    m_languageMenu->addAction(m_chineseLanguageAction);

    // edit menu
    m_editMenu = menuBar()->addMenu(QString());
    m_editMenu->addAction(m_undoAction);
    m_editMenu->addAction(m_redoQtAction);

    // help menu
    m_helpMenu = menuBar()->addMenu(QString());
    m_helpMenu->addAction(m_aboutAction);
    m_helpMenu->addAction(m_aboutQtAction);
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::retranslateStaticTexts() {
    //: Main window title; the version suffix is appended separately in code.
    setWindowTitle(tr("Cullendula") + applicationVersionSuffix());

    if (m_mainMenu != nullptr) {
        //: Top-level menu containing application settings such as extensions, style, and language.
        m_mainMenu->setTitle(tr("Main"));
    }

    if (m_extensionsMenu != nullptr) {
        //: Submenu listing the enabled image filename extensions for directory scanning.
        m_extensionsMenu->setTitle(tr("Extensions"));
    }

    if (m_styleMenu != nullptr) {
        //: Submenu for switching between visual themes.
        m_styleMenu->setTitle(tr("Style"));
    }

    if (m_languageMenu != nullptr) {
        //: Submenu for switching the user-interface language.
        m_languageMenu->setTitle(tr("Language"));
    }

    for (auto it = m_extensionActions.begin(); it != m_extensionActions.end(); ++it) {
        if (it.value() != nullptr) {
            it.value()->setText(it.key().toUpper());
            //: Tooltip for a checkable menu entry that enables a filename extension such as jpg, png, or webp for future directory scans.
            it.value()->setStatusTip(tr("Enable loading of *.%1 files when opening the next directory").arg(it.key()));
        }
    }

    if (m_lightThemeAction != nullptr) {
        //: Menu label for the light visual theme.
        m_lightThemeAction->setText(tr("Light"));
        //: Tooltip for switching the whole application to the light theme.
        m_lightThemeAction->setStatusTip(tr("Use the light application theme"));
    }

    if (m_darkThemeAction != nullptr) {
        //: Menu label for the dark visual theme.
        m_darkThemeAction->setText(tr("Dark"));
        //: Tooltip for switching the whole application to the dark theme.
        m_darkThemeAction->setStatusTip(tr("Use the high-contrast dark application theme"));
    }

    if (m_purpleThemeAction != nullptr) {
        //: Menu label for the gloomy purple visual theme.
        m_purpleThemeAction->setText(tr("Purple"));
        //: Tooltip for switching the whole application to the gloomy purple-and-cyan theme.
        m_purpleThemeAction->setStatusTip(tr("Use the gloomy purple application theme"));
    }

    if (m_englishLanguageAction != nullptr) {
        //: Language menu entry naming the English user-interface language.
        m_englishLanguageAction->setText(tr("English"));
        //: Tooltip for switching back to the original English source texts.
        m_englishLanguageAction->setStatusTip(tr("Use the default English source texts"));
    }

    if (m_germanLanguageAction != nullptr) {
        //: Language menu entry naming the German user-interface language in German.
        m_germanLanguageAction->setText(tr("Deutsch"));
        //: Tooltip for loading the German translation file.
        m_germanLanguageAction->setStatusTip(tr("Load the German user-interface translation"));
    }

    if (m_croatianLanguageAction != nullptr) {
        //: Language menu entry naming the Croatian user-interface language in Croatian.
        m_croatianLanguageAction->setText(tr("Hrvatski"));
        //: Tooltip for loading the Croatian translation file.
        m_croatianLanguageAction->setStatusTip(tr("Load the Croatian user-interface translation"));
    }

    if (m_chineseLanguageAction != nullptr) {
        //: Language menu entry naming the Chinese user-interface language in Chinese.
        m_chineseLanguageAction->setText(tr("中文"));
        //: Tooltip for loading the Simplified Chinese translation file.
        m_chineseLanguageAction->setStatusTip(tr("Load the Chinese user-interface translation"));
    }

    if (m_editMenu != nullptr) {
        //: Top-level menu for undo and redo actions.
        m_editMenu->setTitle(tr("Edit"));
    }

    if (m_undoAction != nullptr) {
        //: Edit menu action label that reverses the most recent file move.
        m_undoAction->setText(tr("Undo"));
        //: Tooltip for the Undo action that moves the previously moved image back to its original location.
        m_undoAction->setStatusTip(tr("Revert the last file-move-operation"));
    }

    if (m_redoQtAction != nullptr) {
        //: Edit menu action label that reapplies the most recently undone file move.
        m_redoQtAction->setText(tr("Redo"));
        //: Tooltip for the Redo action. "undo undo" here means reapplied after an Undo.
        m_redoQtAction->setStatusTip(tr("Redo the last file-move-operation (means: undo undo)"));
    }

    if (m_helpMenu != nullptr) {
        //: Top-level menu for About dialogs and other help-related actions.
        m_helpMenu->setTitle(tr("Help"));
    }

    if (m_aboutAction != nullptr) {
        //: Help menu action label that opens the application's About dialog.
        m_aboutAction->setText(tr("About Cullendula"));
        //: Tooltip for opening the application's About dialog.
        m_aboutAction->setStatusTip(tr("Show the application's About box"));
    }

    if (m_aboutQtAction != nullptr) {
        //: Help menu action label that opens Qt's built-in About dialog.
        m_aboutQtAction->setText(tr("About Qt"));
        //: Tooltip for opening Qt's built-in About dialog.
        m_aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::syncAllowedExtensionsToFileSystemHandler() {
    QStringList enabledExtensions;
    for (auto it = m_extensionActions.cbegin(); it != m_extensionActions.cend(); ++it) {
        if (it.value()->isChecked()) {
            enabledExtensions.append(it.key());
        }
    }

    m_fileSystemHandler.setAllowedImageExtensions(enabledExtensions);
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::applyLanguage(CullendulaAppBootstrap::UiLanguage language) {
    if (!CullendulaAppBootstrap::setApplicationLanguage(language)) {
        //: Status bar error when loading the requested translation catalog failed.
        printStatus(tr("Could not load the selected language."));
        return;
    }

    if (m_englishLanguageAction != nullptr) {
        m_englishLanguageAction->setChecked(language == CullendulaAppBootstrap::UiLanguage::English);
    }

    if (m_germanLanguageAction != nullptr) {
        m_germanLanguageAction->setChecked(language == CullendulaAppBootstrap::UiLanguage::German);
    }

    if (m_croatianLanguageAction != nullptr) {
        m_croatianLanguageAction->setChecked(language == CullendulaAppBootstrap::UiLanguage::Croatian);
    }

    if (m_chineseLanguageAction != nullptr) {
        m_chineseLanguageAction->setChecked(language == CullendulaAppBootstrap::UiLanguage::Chinese);
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::applyTheme(ThemeMode themeMode) {
    m_themeMode = themeMode;
    ThemeDefinition theme;
    QString styleSheet;
    switch (themeMode) {
        case ThemeMode::Light:
            theme = getLightThemeDefinition();
            styleSheet = getStandardThemeStyleSheet(theme);
            break;
        case ThemeMode::Dark:
            theme = getDarkThemeDefinition();
            styleSheet = getStandardThemeStyleSheet(theme);
            break;
        case ThemeMode::Purple:
            theme = getPurpleThemeDefinition();
            styleSheet = getPurpleThemeStyleSheet(theme);
            break;
    }

    if (qApp != nullptr) {
        qApp->setStyle(QStyleFactory::create("Fusion"));
        qApp->setPalette(createThemePalette(theme));
        qApp->setStyleSheet(styleSheet);
    }

    if (m_lightThemeAction != nullptr) {
        m_lightThemeAction->setChecked(themeMode == ThemeMode::Light);
    }

    if (m_darkThemeAction != nullptr) {
        m_darkThemeAction->setChecked(themeMode == ThemeMode::Dark);
    }

    if (m_purpleThemeAction != nullptr) {
        m_purpleThemeAction->setChecked(themeMode == ThemeMode::Purple);
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::updateUndoRedoButtonStatus() {
    if (m_undoAction != nullptr) {
        m_undoAction->setEnabled(m_fileSystemHandler.canUndo());
    }

    if (m_redoQtAction != nullptr) {
        m_redoQtAction->setEnabled(m_fileSystemHandler.canRedo());
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::about() {
    //: Status bar trace shown when the user opens the application's About dialog from the Help menu.
    printStatus(tr("Invoked Help|About"));
    //: Title of the application's About dialog.
    showInformationDialog(tr("About Cullendula"),
                          //: Rich-text body of the application's About dialog.
                          tr("Helper program to sort out (\"cull\") a collection of pictures in a directory after a nice photo-walk or event.<br>"
                             "Should work cross-platform.<br>"
                             "<br>"
                             "Developed by <a href='mail@marcelpetrick.it'>mail@marcelpetrick.it</a><br>"
                             "Source code can be found inside the repository at <a "
                             "href='https://github.com/marcelpetrick/Cullendula/'>https://github.com/marcelpetrick/Cullendula</a><br>"
                             "Feel free to use and share: GPL v3 :3"));
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::aboutQt() {
    //: Status bar trace shown when the user opens Qt's About dialog from the Help menu.
    printStatus(tr("Invoked Help|About Qt"));
    //: Title of the Qt runtime information dialog.
    //: Rich-text body of the Qt runtime information dialog. %1 is the Qt version, %2 is the Qt installation prefix path.
    showInformationDialog(tr("About Qt"), tr("This application currently runs with Qt %1.<br>"
                                             "Qt installation prefix: %2<br>"
                                             "<br>"
                                             "Qt is a cross-platform application framework for building desktop and embedded applications.")
                                              .arg(QString::fromLatin1(qVersion()), QLibraryInfo::path(QLibraryInfo::PrefixPath)));
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::showInformationDialog(QString const& title, QString const& text) {
    auto* messageBox = new QMessageBox(QMessageBox::Information, title, text, QMessageBox::Ok, this);
    messageBox->setAttribute(Qt::WA_DeleteOnClose, true);
    messageBox->setTextFormat(Qt::RichText);
    messageBox->open();
}

//----------------------------------------------------------------------------
