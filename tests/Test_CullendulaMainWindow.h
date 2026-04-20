//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#pragma once

#include "CullendulaMainWindow.h"

#include <QtCore/QTemporaryDir>
#include <QtTest/QtTest>

#include <memory>

class QAction;
class QLabel;
class QPushButton;
class QStatusBar;

class Test_CullendulaMainWindow : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();

    void slot_Test_InitialState();
    void slot_Test_ResizeWithoutLoadedImages_ShowsFallbackText();
    void slot_Test_DragEnterAndDropValidDirectory_LoadsImages();
    void slot_Test_DropInvalidPayload_ShowsErrorStatus();
    void slot_Test_ButtonNavigationAndSaveFlow();
    void slot_Test_ButtonTrashFlow();
    void slot_Test_UndoRedoActions_MoveFilesOnDisk();
    void slot_Test_AboutAction_ShowsDialogAndStatus();

private:
    QString createImage(QString const& relativePath, QColor const& color = Qt::red);
    void sendDropWithUrls(QList<QUrl> const& urls);
    QAction* findAction(QString const& text) const;
    QPushButton* findButton(char const* name) const;
    QLabel* findCenterLabel() const;
    QStatusBar* findStatusBar() const;

    std::unique_ptr<QTemporaryDir> m_tempDir;
    std::unique_ptr<CullendulaMainWindow> m_window;
};
