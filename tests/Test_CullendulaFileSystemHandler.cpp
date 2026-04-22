//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#include "Test_CullendulaFileSystemHandler.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

namespace {
bool pathDoesNotExist(QString const&) { return false; }

bool pathIsNotDirectory(QString const&) { return false; }

bool mkdirFails(QDir&, QString const&) { return false; }

bool mkdirSucceeds(QDir&, QString const&) { return true; }
}  // namespace

//----------------------------------------------------------------------------------

QString Test_CullendulaFileSystemHandler::createFile(QString const& relativePath) {
    QString const absolutePath = m_tempDir->path() + QDir::separator() + relativePath;

    QFileInfo const fileInfo(absolutePath);
    QDir().mkpath(fileInfo.absolutePath());

    QFile file(absolutePath);
    bool const opened = file.open(QIODevice::WriteOnly);
    if (!opened) {
        QTest::qFail("Could not create test file", __FILE__, __LINE__);
        return {};
    }
    file.write("test");
    file.close();

    return absolutePath;
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::createImageSet() {
    createFile("alpha.jpg");
    createFile("beta.jpeg");
    createFile("notes.txt");
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::init() {
    m_tempDir = std::make_unique<QTemporaryDir>();
    QVERIFY(m_tempDir->isValid());
    m_handler = std::make_unique<CullendulaFileSystemHandler>();
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::cleanup() {
    m_handler.reset();
    m_tempDir.reset();
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SetWorkingPath_FromDirectory() {
    createImageSet();

    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("alpha.jpg"));
    QCOMPARE(m_handler->getCurrentStatus(), QString("showing 1 of 2"));
    QVERIFY(QDir(m_tempDir->path() + QDir::separator() + "output").exists());
    QVERIFY(QDir(m_tempDir->path() + QDir::separator() + "trash").exists());
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SetWorkingPath_FromImageFile() {
    QString const imagePath = createFile("subdir/alpha.jpg");
    createFile("subdir/beta.jpeg");

    QVERIFY(m_handler->setWorkingPath(imagePath));
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).absolutePath(), QFileInfo(imagePath).absolutePath());
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("alpha.jpg"));
    QVERIFY(QDir(QFileInfo(imagePath).absolutePath() + QDir::separator() + "output").exists());
    QVERIFY(QDir(QFileInfo(imagePath).absolutePath() + QDir::separator() + "trash").exists());
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_GetSuggestedImageExtensions_IsNormalizedAndBounded() {
    QStringList const suggestedExtensions = CullendulaFileSystemHandler::getSuggestedImageExtensions();

    QVERIFY(!suggestedExtensions.isEmpty());
    QVERIFY(suggestedExtensions.size() <= 10);

    QSet<QString> seenExtensions;
    for (QString const& extension : suggestedExtensions) {
        QCOMPARE(extension, extension.toLower());
        QVERIFY(!seenExtensions.contains(extension));
        seenExtensions.insert(extension);
    }
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_GetSuggestedImageExtensions_AppendsAdditionalSupportedFormats() {
    QSet<QString> const supportedSuffixes = {"png", "jpg", "avif", "heic"};

    QStringList const suggestedExtensions = CullendulaFileSystemHandlerDetail::getSuggestedImageExtensions(supportedSuffixes);

    QCOMPARE(suggestedExtensions, QStringList({"png", "jpg", "avif", "heic"}));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_GetSuggestedImageExtensions_StopsWhenAdditionalFormatsReachLimit() {
    QSet<QString> const supportedSuffixes = {"png", "jpg", "avif", "heic", "jxl", "pbm", "pgm", "ppm", "xbm", "xpm", "cur"};

    QStringList const suggestedExtensions = CullendulaFileSystemHandlerDetail::getSuggestedImageExtensions(supportedSuffixes);

    QCOMPARE(suggestedExtensions.size(), 10);
    QVERIFY(suggestedExtensions.contains("png"));
    QVERIFY(suggestedExtensions.contains("jpg"));
    QVERIFY(suggestedExtensions.contains("cur"));
    QVERIFY(!suggestedExtensions.contains("xpm"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SetAllowedImageExtensions_NormalizesAndDropsUnsupportedValues() {
    m_handler->setAllowedImageExtensions({" PNG ", "jpg", "JPG", "not-an-image-format", ""});

    QStringList const allowedExtensions = m_handler->getAllowedImageExtensions();
    QCOMPARE(allowedExtensions, QStringList({"jpg", "png"}));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SetAllowedImageExtensions_FiltersFiles() {
    createFile("alpha.jpg");
    createFile("beta.png");
    createFile("gamma.jpeg");

    m_handler->setAllowedImageExtensions({"png"});
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("beta.png"));

    m_handler->setAllowedImageExtensions({"JPG", "JPEG"});
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("alpha.jpg"));
    QCOMPARE(m_handler->getCurrentStatus(), QString("showing 1 of 2"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SetWorkingPath_FindsPngAndUppercaseSuffixes() {
    createFile("alpha.JPG");
    createFile("beta.png");
    createFile("notes.txt");

    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("alpha.JPG"));
    QCOMPARE(m_handler->getCurrentStatus(), QString("showing 1 of 2"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SetWorkingPath_InvalidPath() {
    QVERIFY(!m_handler->setWorkingPath(m_tempDir->path() + QDir::separator() + "missing"));
    QVERIFY(m_handler->getCurrentImagePath().isEmpty());
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SetWorkingPath_PathWithMissingParentDirectory_ReturnsError() {
    QString const missingPath = m_tempDir->path() + QDir::separator() + "missing" + QDir::separator() + "alpha.jpg";

    QVERIFY(!m_handler->setWorkingPath(missingPath));
    QVERIFY(m_handler->getCurrentImagePath().isEmpty());
    QVERIFY(m_handler->getLastErrorMessage().contains("could not be resolved to an existing directory"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SetWorkingPath_FailsWhenOutputDirectoryCannotBeCreated() {
    createImageSet();
    createFile("output");

    QVERIFY(!m_handler->setWorkingPath(m_tempDir->path()));
    QVERIFY(QFileInfo(m_tempDir->path() + QDir::separator() + "output").isFile());
    QVERIFY(QDir(m_tempDir->path() + QDir::separator() + "trash").exists());
    QVERIFY(m_handler->getLastErrorMessage().contains("Could not prepare 'output' directory"));
    QVERIFY(m_handler->getLastErrorMessage().contains("non-directory"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SetWorkingPath_FailsWhenTrashDirectoryCannotBeCreated() {
    createImageSet();
    createFile("trash");

    QVERIFY(!m_handler->setWorkingPath(m_tempDir->path()));
    QVERIFY(QDir(m_tempDir->path() + QDir::separator() + "output").exists());
    QVERIFY(QFileInfo(m_tempDir->path() + QDir::separator() + "trash").isFile());
    QVERIFY(m_handler->getLastErrorMessage().contains("Could not prepare 'trash' directory"));
    QVERIFY(m_handler->getLastErrorMessage().contains("non-directory"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SaveCurrentFile_RecreatesMissingOutputDirectory() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QDir outputDir(m_tempDir->path() + QDir::separator() + "output");
    QVERIFY(outputDir.removeRecursively());
    QVERIFY(!outputDir.exists());

    QVERIFY(m_handler->saveCurrentFile());
    QVERIFY(outputDir.exists());
    QVERIFY(QFile::exists(outputDir.path() + QDir::separator() + "alpha.jpg"));
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("beta.jpeg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_TrashCurrentFile_RecreatesMissingTrashDirectory() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QDir trashDir(m_tempDir->path() + QDir::separator() + "trash");
    QVERIFY(trashDir.removeRecursively());
    QVERIFY(!trashDir.exists());

    QVERIFY(m_handler->trashCurrentFile());
    QVERIFY(trashDir.exists());
    QVERIFY(QFile::exists(trashDir.path() + QDir::separator() + "alpha.jpg"));
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("beta.jpeg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SaveCurrentFile_FailsWhenOutputDirectoryPathIsBlockedAfterLoad() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QDir outputDir(m_tempDir->path() + QDir::separator() + "output");
    QVERIFY(outputDir.removeRecursively());
    QVERIFY(createFile("output") == m_tempDir->path() + QDir::separator() + "output");

    QString const originalCurrent = m_handler->getCurrentImagePath();
    QVERIFY(!originalCurrent.isEmpty());

    QVERIFY(!m_handler->saveCurrentFile());
    QCOMPARE(m_handler->getCurrentImagePath(), originalCurrent);
    QVERIFY(m_handler->getLastErrorMessage().contains("Could not prepare 'output' directory"));
    QVERIFY(m_handler->getLastErrorMessage().contains("non-directory"));
    QVERIFY(QFile::exists(originalCurrent));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SetWorkingPath_NoImages() {
    createFile("notes.txt");

    QVERIFY(!m_handler->setWorkingPath(m_tempDir->path()));
    QVERIFY(m_handler->getCurrentImagePath().isEmpty());
    QVERIFY(QDir(m_tempDir->path() + QDir::separator() + "output").exists());
    QVERIFY(QDir(m_tempDir->path() + QDir::separator() + "trash").exists());
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SetWorkingPath_ClearsStateOnReloadFailure() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));
    QVERIFY(m_handler->saveCurrentFile());
    QVERIFY(!m_handler->getCurrentImagePath().isEmpty());
    QVERIFY(m_handler->canUndo());

    QTemporaryDir emptyDir;
    QVERIFY(emptyDir.isValid());

    QVERIFY(!m_handler->setWorkingPath(emptyDir.path()));
    QVERIFY(m_handler->getCurrentImagePath().isEmpty());
    QVERIFY(!m_handler->canUndo());
    QVERIFY(!m_handler->canRedo());
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_GetCurrentImagePath_ReturnsEmptyWhenCurrentFileWasDeleted() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QString const currentPath = m_handler->getCurrentImagePath();
    QVERIFY(!currentPath.isEmpty());
    QVERIFY(QFile::remove(currentPath));

    QVERIFY(m_handler->getCurrentImagePath().isEmpty());
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_NavigationWrapsAround() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("alpha.jpg"));
    QVERIFY(m_handler->switchCurrentPositionToTheLeft());
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("beta.jpeg"));
    QVERIFY(m_handler->switchCurrentPositionToTheRight());
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("alpha.jpg"));
    QVERIFY(m_handler->switchCurrentPositionToTheRight());
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("beta.jpeg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SaveCurrentFile_MovesFileAndUpdatesState() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QString const originalCurrent = m_handler->getCurrentImagePath();
    QVERIFY(m_handler->saveCurrentFile());

    QVERIFY(!QFile::exists(originalCurrent));
    QVERIFY(QFile::exists(m_tempDir->path() + QDir::separator() + "output" + QDir::separator() + "alpha.jpg"));
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("beta.jpeg"));
    QCOMPARE(m_handler->getCurrentStatus(), QString("showing 1 of 1"));
    QVERIFY(m_handler->canUndo());
    QVERIFY(!m_handler->canRedo());
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SaveCurrentFile_LastImageClearsCurrentSelection() {
    createFile("alpha.jpg");
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QVERIFY(m_handler->saveCurrentFile());
    QVERIFY(m_handler->getCurrentImagePath().isEmpty());
    QCOMPARE(m_handler->m_positionCurrentFile, -1);
    QVERIFY(m_handler->canUndo());
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_TrashCurrentFile_MovesFileAndUpdatesState() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QVERIFY(m_handler->trashCurrentFile());

    QVERIFY(QFile::exists(m_tempDir->path() + QDir::separator() + "trash" + QDir::separator() + "alpha.jpg"));
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("beta.jpeg"));
    QCOMPARE(m_handler->getCurrentStatus(), QString("showing 1 of 1"));
    QVERIFY(m_handler->canUndo());
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_UndoRedo_MoveFilesOnDisk() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QString const originalPath = m_handler->getCurrentImagePath();
    QString const movedPath = m_tempDir->path() + QDir::separator() + "output" + QDir::separator() + "alpha.jpg";

    QVERIFY(m_handler->saveCurrentFile());
    QVERIFY(QFile::exists(movedPath));
    QVERIFY(m_handler->canUndo());
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("beta.jpeg"));
    QCOMPARE(m_handler->getCurrentStatus(), QString("showing 1 of 1"));

    QVERIFY(m_handler->undo());
    QVERIFY(QFile::exists(originalPath));
    QVERIFY(!QFile::exists(movedPath));
    QVERIFY(m_handler->canRedo());
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("alpha.jpg"));
    QCOMPARE(m_handler->getCurrentStatus(), QString("showing 1 of 2"));
    QVERIFY(m_handler->switchCurrentPositionToTheRight());
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("beta.jpeg"));

    QVERIFY(m_handler->redo());
    QVERIFY(!QFile::exists(originalPath));
    QVERIFY(QFile::exists(movedPath));
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("beta.jpeg"));
    QCOMPARE(m_handler->getCurrentStatus(), QString("showing 1 of 1"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SaveCurrentFile_UsesUniqueNameOnCollision() {
    createImageSet();
    createFile("output/alpha.jpg");
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QString const originalCurrent = m_handler->getCurrentImagePath();
    QVERIFY(m_handler->saveCurrentFile());

    QVERIFY(!QFile::exists(originalCurrent));
    QVERIFY(QFile::exists(m_tempDir->path() + QDir::separator() + "output" + QDir::separator() + "alpha.jpg"));
    QVERIFY(QFile::exists(m_tempDir->path() + QDir::separator() + "output" + QDir::separator() + "alpha (1).jpg"));
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("beta.jpeg"));
    QVERIFY(m_handler->canUndo());
    QVERIFY(!m_handler->canRedo());
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SaveCurrentFile_UsesNextFreeCollisionSuffix() {
    createImageSet();
    createFile("output/alpha.jpg");
    createFile("output/alpha (1).jpg");
    createFile("output/alpha (2).jpg");
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QVERIFY(m_handler->saveCurrentFile());

    QVERIFY(QFile::exists(m_tempDir->path() + QDir::separator() + "output" + QDir::separator() + "alpha (3).jpg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SaveCurrentFile_WithoutLoadedImage_ReturnsError() {
    QVERIFY(!m_handler->saveCurrentFile());
    QCOMPARE(m_handler->getLastErrorMessage(), QString("No current image is available to move."));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SaveCurrentFile_RenameFailure_ReturnsError() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QString const originalCurrent = m_handler->getCurrentImagePath();
    QVERIFY(!originalCurrent.isEmpty());
    QVERIFY(QFile::remove(originalCurrent));

    QVERIFY(!m_handler->saveCurrentFile());
    QVERIFY(m_handler->getLastErrorMessage().contains("Could not move 'alpha.jpg' to 'output'"));
    QVERIFY(m_handler->getLastErrorMessage().contains("rename operation failed"));
    QVERIFY(!m_handler->canUndo());
    QVERIFY(QFile::exists(m_tempDir->path() + QDir::separator() + "beta.jpeg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_TrashCurrentFile_UsesUniqueNameOnCollision() {
    createImageSet();
    createFile("trash/alpha.jpg");
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QVERIFY(m_handler->trashCurrentFile());

    QVERIFY(QFile::exists(m_tempDir->path() + QDir::separator() + "trash" + QDir::separator() + "alpha.jpg"));
    QVERIFY(QFile::exists(m_tempDir->path() + QDir::separator() + "trash" + QDir::separator() + "alpha (1).jpg"));
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("beta.jpeg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_TrashCurrentFile_RenameFailure_ReturnsError() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QString const originalCurrent = m_handler->getCurrentImagePath();
    QVERIFY(!originalCurrent.isEmpty());
    QVERIFY(QFile::remove(originalCurrent));

    QVERIFY(!m_handler->trashCurrentFile());
    QVERIFY(m_handler->getLastErrorMessage().contains("Could not move 'alpha.jpg' to 'trash'"));
    QVERIFY(m_handler->getLastErrorMessage().contains("rename operation failed"));
    QVERIFY(!m_handler->canUndo());
    QVERIFY(QFile::exists(m_tempDir->path() + QDir::separator() + "beta.jpeg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_Undo_WithoutHistory_ReturnsError() {
    QVERIFY(!m_handler->undo());
    QCOMPARE(m_handler->getLastErrorMessage(), QString("No undo step is currently available."));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_Redo_WithoutHistory_ReturnsError() {
    QVERIFY(!m_handler->redo());
    QCOMPARE(m_handler->getLastErrorMessage(), QString("No redo step is currently available."));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_Undo_WhenMovedFileIsMissing_ReturnsFalse() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));
    QVERIFY(m_handler->saveCurrentFile());

    QString const movedPath = m_tempDir->path() + QDir::separator() + "output" + QDir::separator() + "alpha.jpg";
    QVERIFY(QFile::exists(movedPath));
    QVERIFY(QFile::remove(movedPath));

    QVERIFY(!m_handler->undo());
    QVERIFY(m_handler->getLastErrorMessage().contains("Could not move 'alpha.jpg' to 'undo'"));
    QVERIFY(!QFile::exists(m_tempDir->path() + QDir::separator() + "alpha.jpg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_Undo_FailurePreservesUndoAndRedoHistory() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));
    QVERIFY(m_handler->saveCurrentFile());

    QString const movedPath = m_tempDir->path() + QDir::separator() + "output" + QDir::separator() + "alpha.jpg";
    QVERIFY(QFile::exists(movedPath));
    QVERIFY(QFile::remove(movedPath));

    QVERIFY(m_handler->canUndo());
    QVERIFY(!m_handler->canRedo());
    QVERIFY(!m_handler->undo());
    QVERIFY(m_handler->canUndo());
    QVERIFY(!m_handler->canRedo());

    QVERIFY(createFile("output/alpha.jpg") == movedPath);
    QVERIFY(m_handler->undo());
    QVERIFY(QFile::exists(m_tempDir->path() + QDir::separator() + "alpha.jpg"));
    QVERIFY(!m_handler->canUndo());
    QVERIFY(m_handler->canRedo());
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_Redo_WhenRestoredFileIsMissing_ReturnsFalse() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));
    QVERIFY(m_handler->saveCurrentFile());
    QVERIFY(m_handler->undo());

    QString const restoredPath = m_tempDir->path() + QDir::separator() + "alpha.jpg";
    QVERIFY(QFile::exists(restoredPath));
    QVERIFY(QFile::remove(restoredPath));

    QVERIFY(!m_handler->redo());
    QVERIFY(m_handler->getLastErrorMessage().contains("Could not move 'alpha.jpg' to 'redo'"));
    QVERIFY(!QFile::exists(m_tempDir->path() + QDir::separator() + "output" + QDir::separator() + "alpha.jpg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_Redo_FailurePreservesUndoAndRedoHistory() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));
    QVERIFY(m_handler->saveCurrentFile());
    QVERIFY(m_handler->undo());

    QString const restoredPath = m_tempDir->path() + QDir::separator() + "alpha.jpg";
    QVERIFY(QFile::exists(restoredPath));
    QVERIFY(QFile::remove(restoredPath));

    QVERIFY(!m_handler->canUndo());
    QVERIFY(m_handler->canRedo());
    QVERIFY(!m_handler->redo());
    QVERIFY(!m_handler->canUndo());
    QVERIFY(m_handler->canRedo());

    QVERIFY(createFile("alpha.jpg") == restoredPath);
    QVERIFY(m_handler->redo());
    QVERIFY(QFile::exists(m_tempDir->path() + QDir::separator() + "output" + QDir::separator() + "alpha.jpg"));
    QVERIFY(m_handler->canUndo());
    QVERIFY(!m_handler->canRedo());
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_RebuildImageFileList_MissingPreferredPathUsesFallbackPosition() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QVERIFY(m_handler->rebuildImageFileList(m_tempDir->path() + QDir::separator() + "missing.jpg", 1));
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("beta.jpeg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_Redo_MissingSourceIndexUsesCurrentFallbackPosition() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));
    QVERIFY(m_handler->saveCurrentFile());
    QVERIFY(m_handler->undo());

    m_handler->m_currentImages = {QFileInfo(m_tempDir->path() + QDir::separator() + "beta.jpeg")};
    m_handler->m_positionCurrentFile = 0;

    QVERIFY(m_handler->redo());
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("beta.jpeg"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_CreateOutputFolder_HelperReportsMkdirFailure() {
    QDir workingDir(m_tempDir->path());
    QString errorMessage;
    CullendulaFileSystemHandlerDetail::OutputFolderHooks const hooks{pathDoesNotExist, pathIsNotDirectory, pathDoesNotExist, mkdirFails};

    QVERIFY(!CullendulaFileSystemHandlerDetail::createOutputFolder(workingDir, "output", errorMessage, hooks));
    QVERIFY(errorMessage.contains("Could not prepare 'output' directory"));
    QVERIFY(errorMessage.contains("creating the directory failed"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_CreateOutputFolder_HelperReportsMissingDirectoryAfterCreation() {
    QDir workingDir(m_tempDir->path());
    QString errorMessage;
    CullendulaFileSystemHandlerDetail::OutputFolderHooks const hooks{pathDoesNotExist, pathIsNotDirectory, pathDoesNotExist, mkdirSucceeds};

    QVERIFY(!CullendulaFileSystemHandlerDetail::createOutputFolder(workingDir, "trash", errorMessage, hooks));
    QVERIFY(errorMessage.contains("Could not prepare 'trash' directory"));
    QVERIFY(errorMessage.contains("still missing after creation"));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_CheckInternalSanity_FailsForOutOfRangeIndex() {
    createImageSet();
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    m_handler->m_positionCurrentFile = m_handler->m_currentImages.size();

    QVERIFY(!m_handler->checkInternalSanity());
}
