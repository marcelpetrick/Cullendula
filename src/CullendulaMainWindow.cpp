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
#include <QtCore/QLibraryInfo>
#include <QtCore/QMimeData>
#include <QtGui/QAction>
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
//! v0.1 was the basic release; working, but ugly
//! v0.2 improved useability and stability; more features (move to trash!); refactored code-base; improved code-quality
//! v0.3 added tooltips; fixed the "pumping center-label"-issue; added menus; fixed some resizing-issues with the image-label
//! v0.4 added undo/redo-functionality with unit-test; added a nice violet icon for the executable and program
//! v0.5 moved the buildsystem to cmake (from qmake)
QString const c_versionString = " - v0.6.12";

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

QString getThemeStyleSheet(ThemeDefinition const& theme) {
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
}  // namespace

//----------------------------------------------------------------------------

CullendulaMainWindow::CullendulaMainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::CullendulaMainWindow) {
    setAcceptDrops(true);
    ui->setupUi(this);

    // make the current version information visible to the user - as long as no menu with help&stuff exists
    setWindowTitle(windowTitle().append(c_versionString));

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
    applyTheme(ThemeMode::Light);
    syncAllowedExtensionsToFileSystemHandler();

    // set undo/redo correctly
    updateUndoRedoButtonStatus();

    printStatus("system is up and running :)");
}

//----------------------------------------------------------------------------

CullendulaMainWindow::~CullendulaMainWindow() { delete ui; }

//----------------------------------------------------------------------------

CullendulaMainWindow::ThemeMode CullendulaMainWindow::getThemeMode() const { return m_themeMode; }

//----------------------------------------------------------------------------

void CullendulaMainWindow::dragEnterEvent(QDragEnterEvent* event) {
    event->accept();

    printStatus("drop current load and let's see what you dragged?");
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
                qDebug() << "\tNo matching image files found for the current filter";
            }
        }
    } else {
        printStatus("The load was not usable! :(");
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
        qDebug() << "\tERROR: moving file did not succeed";
    }
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::loadAndScalePhoto(QString const& path) const {
    qDebug() << "CullendulaMainWindow::loadAndScalePhoto(): path=" << path;

    QPixmap const pixmap = QPixmap(path);

    // scale while keeping the aspect ratio
    int width = ui->centerLabel->width() - c_extraPixelsBecauseOfFraming;
    int height = ui->centerLabel->height() - c_extraPixelsBecauseOfFraming;
    qDebug() << "label-size:" << width << "*" << height;

    // prevent upscaling of smaller photos
    if (pixmap.width() < width) {
        width = pixmap.width();
    }

    if (pixmap.height() < height) {
        height = pixmap.height();
    }

    // assert that both values are positive
    width = std::max(0, width);
    height = std::max(0, height);

    // set a scaled pixmap keeping its aspect ratio
    ui->centerLabel->setPixmap(pixmap.scaled(width, height, Qt::KeepAspectRatio));
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::refreshLabel() {
    qDebug() << "CullendulaMainWindow::refreshLabel():";

    QString const path = m_fileSystemHandler.getCurrentImagePath();
    if (path.isEmpty())  // just the case if no valid images found
    {
        ui->centerLabel->setText("no more valid images found: work maybe finished? :)\ndrag&drop the next folder or files if you want!");

        activateButtons(false);

        printStatus("no more files");
    } else {
        if (QFile::exists(path)) {
            // load the file
            loadAndScalePhoto(path);

            activateButtons(true);

            // print the current file-path as user-notification
            QString const message = m_fileSystemHandler.getCurrentStatus() + ": " + path;
            printStatus(message);
        } else {
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
        extensionAction->setStatusTip(tr("Enable loading of *.%1 files when opening the next directory").arg(extension));
        connect(extensionAction, &QAction::toggled, this, [this]() { syncAllowedExtensionsToFileSystemHandler(); });
        m_extensionActions.insert(extension, extensionAction);
    }

    m_lightThemeAction = new QAction(tr("Light"), this);
    m_lightThemeAction->setCheckable(true);
    m_lightThemeAction->setObjectName("themeAction_light");
    m_lightThemeAction->setStatusTip(tr("Use the light application theme"));
    connect(m_lightThemeAction, &QAction::triggered, this, [this]() { applyTheme(ThemeMode::Light); });

    m_darkThemeAction = new QAction(tr("Dark"), this);
    m_darkThemeAction->setCheckable(true);
    m_darkThemeAction->setObjectName("themeAction_dark");
    m_darkThemeAction->setStatusTip(tr("Use the high-contrast dark application theme"));
    connect(m_darkThemeAction, &QAction::triggered, this, [this]() { applyTheme(ThemeMode::Dark); });

    // edit menu
    m_undoAction = new QAction(tr("Undo"), this);
    m_undoAction->setStatusTip(tr("Revert the last file-move-operation"));
    m_undoAction->setShortcut(Qt::CTRL | Qt::Key_Y);
    connect(m_undoAction, &QAction::triggered, this, [=]() {
        qDebug("pressed Undo");
        if (m_fileSystemHandler.undo()) {
            refreshLabel();
        }
        updateUndoRedoButtonStatus();
    });

    m_redoQtAction = new QAction(tr("Redo"), this);
    m_redoQtAction->setStatusTip(tr("Redo the last file-move-operation (means: undo undo)"));
    m_redoQtAction->setShortcut(Qt::CTRL | Qt::Key_Z);
    connect(m_redoQtAction, &QAction::triggered, this, [=]() {
        qDebug("pressed Redo");
        if (m_fileSystemHandler.redo()) {
            refreshLabel();
        }
        updateUndoRedoButtonStatus();
    });

    // help menu
    m_aboutAction = new QAction(tr("About Cullendula"), this);
    m_aboutAction->setStatusTip(tr("Show the application's About box"));
    m_aboutAction->setShortcut(Qt::CTRL | Qt::Key_A);
    connect(m_aboutAction, &QAction::triggered, this, &CullendulaMainWindow::about);

    m_aboutQtAction = new QAction(tr("About Qt"), this);
    m_aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    m_aboutQtAction->setShortcut(Qt::CTRL | Qt::Key_Q);
    connect(m_aboutQtAction, &QAction::triggered, this, &CullendulaMainWindow::aboutQt);
}

//----------------------------------------------------------------------------

void CullendulaMainWindow::createMenus() {
    // main menu
    m_mainMenu = menuBar()->addMenu(tr("Main"));
    m_extensionsMenu = m_mainMenu->addMenu(tr("Extensions"));
    for (QAction* extensionAction : m_extensionActions) {
        m_extensionsMenu->addAction(extensionAction);
    }
    m_styleMenu = m_mainMenu->addMenu(tr("Style"));
    m_styleMenu->addAction(m_lightThemeAction);
    m_styleMenu->addAction(m_darkThemeAction);

    // edit menu
    m_editMenu = menuBar()->addMenu(tr("Edit"));
    m_editMenu->addAction(m_undoAction);
    m_editMenu->addAction(m_redoQtAction);

    // help menu
    m_helpMenu = menuBar()->addMenu(tr("Help"));
    m_helpMenu->addAction(m_aboutAction);
    m_helpMenu->addAction(m_aboutQtAction);
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

void CullendulaMainWindow::applyTheme(ThemeMode themeMode) {
    m_themeMode = themeMode;
    ThemeDefinition const theme = (themeMode == ThemeMode::Dark) ? getDarkThemeDefinition() : getLightThemeDefinition();

    if (qApp != nullptr) {
        qApp->setStyle(QStyleFactory::create("Fusion"));
        qApp->setPalette(createThemePalette(theme));
        qApp->setStyleSheet(getThemeStyleSheet(theme));
    }

    if (m_lightThemeAction != nullptr) {
        m_lightThemeAction->setChecked(themeMode == ThemeMode::Light);
    }

    if (m_darkThemeAction != nullptr) {
        m_darkThemeAction->setChecked(themeMode == ThemeMode::Dark);
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
    printStatus(tr("Invoked Help|About"));
    showInformationDialog(tr("About Cullendula"),
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
    printStatus(tr("Invoked Help|About Qt"));
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
