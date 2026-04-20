//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#pragma once

#include <QtTest/QtTest>
#include <memory>

#include "CullendulaFileSystemHandler.h"

class Test_CullendulaFileSystemHandler : public QObject {
    Q_OBJECT

   private Q_SLOTS:
    void init();
    void cleanup();

    void slot_Test_SetWorkingPath_FromDirectory();
    void slot_Test_SetWorkingPath_FromImageFile();
    void slot_Test_GetSuggestedImageExtensions_IsNormalizedAndBounded();
    void slot_Test_SetAllowedImageExtensions_NormalizesAndDropsUnsupportedValues();
    void slot_Test_SetAllowedImageExtensions_FiltersFiles();
    void slot_Test_SetWorkingPath_FindsPngAndUppercaseSuffixes();
    void slot_Test_SetWorkingPath_InvalidPath();
    void slot_Test_SetWorkingPath_NoImages();
    void slot_Test_SetWorkingPath_ClearsStateOnReloadFailure();
    void slot_Test_NavigationWrapsAround();
    void slot_Test_SaveCurrentFile_MovesFileAndUpdatesState();
    void slot_Test_TrashCurrentFile_MovesFileAndUpdatesState();
    void slot_Test_UndoRedo_MoveFilesOnDisk();
    void slot_Test_SaveCurrentFile_FailsOnNameCollision();

   private:
    QString createFile(QString const& relativePath);
    void createImageSet();

    std::unique_ptr<QTemporaryDir> m_tempDir;
    std::unique_ptr<CullendulaFileSystemHandler> m_handler;
};
