//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

#include "Test_CullendulaFileSystemHandler.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

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

    m_handler->undo();
    QVERIFY(QFile::exists(originalPath));
    QVERIFY(!QFile::exists(movedPath));
    QVERIFY(m_handler->canRedo());

    m_handler->redo();
    QVERIFY(!QFile::exists(originalPath));
    QVERIFY(QFile::exists(movedPath));
}

//----------------------------------------------------------------------------------

void Test_CullendulaFileSystemHandler::slot_Test_SaveCurrentFile_FailsOnNameCollision() {
    createImageSet();
    createFile("output/alpha.jpg");
    QVERIFY(m_handler->setWorkingPath(m_tempDir->path()));

    QString const originalCurrent = m_handler->getCurrentImagePath();
    QVERIFY(!m_handler->saveCurrentFile());

    QVERIFY(QFile::exists(originalCurrent));
    QCOMPARE(QFileInfo(m_handler->getCurrentImagePath()).fileName(), QString("alpha.jpg"));
    QVERIFY(!m_handler->canUndo());
    QVERIFY(!m_handler->canRedo());
}
