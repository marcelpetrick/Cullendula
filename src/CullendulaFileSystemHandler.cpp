//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

// own includes
#include "CullendulaFileSystemHandler.h"

// Qt includes
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QList>
#include <QtGui/QImageReader>
#include <algorithm>

//----------------------------------------------------------------------------

// for constants
namespace {
//! Determines the name of the output-folders
QString const c_hardcodedOutput = "output";
QString const c_hardcodedTrash = "trash";
int const c_maxSuggestedExtensions = 10;

QSet<QString> getSupportedImageSuffixes() {
    QSet<QString> suffixes;

    QList<QByteArray> const supportedFormats = QImageReader::supportedImageFormats();
    for (QByteArray const& format : supportedFormats) {
        suffixes.insert(QString::fromLatin1(format).toLower());
    }

    return suffixes;
}

QStringList normalizeExtensions(QStringList const& extensions) {
    QSet<QString> const supportedSuffixes = getSupportedImageSuffixes();
    QStringList normalizedExtensions;

    for (QString const& extension : extensions) {
        QString const normalizedExtension = extension.trimmed().toLower();
        if (!normalizedExtension.isEmpty() && supportedSuffixes.contains(normalizedExtension) && !normalizedExtensions.contains(normalizedExtension)) {
            normalizedExtensions.append(normalizedExtension);
        }
    }

    return normalizedExtensions;
}

QString formatMoveErrorMessage(QString const& fileName, QString const& subdir, QString const& details) {
    return QString("Could not move '%1' to '%2': %3").arg(fileName, subdir, details);
}
}  // namespace

//----------------------------------------------------------------------------

CullendulaFileSystemHandler::CullendulaFileSystemHandler() : m_workingPath("") {
    setAllowedImageExtensions(getSuggestedImageExtensions());
    qDebug() << "CullendulaFileSystemHandler::CullendulaFileSystemHandler()";
}

//----------------------------------------------------------------------------

QStringList CullendulaFileSystemHandler::getSuggestedImageExtensions() {
    QStringList const preferredExtensions = {"png", "jpg", "jpeg", "webp", "gif", "bmp", "tif", "tiff", "svg", "ico"};

    QStringList suggestedExtensions = normalizeExtensions(preferredExtensions);
    if (suggestedExtensions.size() >= c_maxSuggestedExtensions) {
        suggestedExtensions.resize(c_maxSuggestedExtensions);
        return suggestedExtensions;
    }

    QStringList additionalExtensions = getSupportedImageSuffixes().values();
    std::sort(additionalExtensions.begin(), additionalExtensions.end());

    for (QString const& extension : additionalExtensions) {
        if (!suggestedExtensions.contains(extension)) {
            suggestedExtensions.append(extension);
            if (suggestedExtensions.size() >= c_maxSuggestedExtensions) {
                break;
            }
        }
    }

    return suggestedExtensions;
}

//----------------------------------------------------------------------------

void CullendulaFileSystemHandler::setAllowedImageExtensions(QStringList const& extensions) {
    QStringList const normalizedExtensions = normalizeExtensions(extensions);
    m_allowedImageExtensions = QSet<QString>(normalizedExtensions.cbegin(), normalizedExtensions.cend());
}

//----------------------------------------------------------------------------

QStringList CullendulaFileSystemHandler::getAllowedImageExtensions() const {
    QStringList extensions = m_allowedImageExtensions.values();
    std::sort(extensions.begin(), extensions.end());
    return extensions;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::setWorkingPath(const QString& urlPath) {
    bool returnValue(false);

    qDebug() << "CullendulaFileSystemHandler::setWorkingPath(): urlPath=" << urlPath;
    resetCurrentState();
    m_workingPath.setPath("");
    m_workingPath.setPath(urlPath);

    // trigger now the follow-up
    returnValue = processNewPath();

    return returnValue;
}

//----------------------------------------------------------------------------

void CullendulaFileSystemHandler::resetCurrentState() {
    m_currentImages.clear();
    m_positionCurrentFile = -1;
    m_undoStack = CullendulaUndoStack();
    clearLastErrorMessage();
}

//----------------------------------------------------------------------------

QString CullendulaFileSystemHandler::getCurrentImagePath() {
    QString returnValue;
    qDebug() << "CullendulaFileSystemHandler::getCurrentImagePath():";

    // some defensive checks
    if (checkInternalSanity()) {
        // check for existence
        QString const path(m_currentImages[m_positionCurrentFile].absoluteFilePath());
        qDebug() << "\tpath:" << path;
        QFile const tempFile(path);
        if (tempFile.exists() /* && tempFile.isReadable()*/)  // latter is not the case with Ext4 here ..
        {
            returnValue = path;
        }
    }

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::adjustCurrentPositionBy(int const offset) {
    bool const returnValue = checkInternalSanity();

    if (checkInternalSanity()) {
        m_positionCurrentFile = (m_positionCurrentFile + m_currentImages.size() + offset) % m_currentImages.size();
    }

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::switchCurrentPositionToTheLeft() {
    bool const returnValue = adjustCurrentPositionBy(-1);

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::switchCurrentPositionToTheRight() {
    bool const returnValue = adjustCurrentPositionBy(1);

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::saveCurrentFile() {
    bool returnValue(false);
    qDebug() << "CullendulaFileSystemHandler::saveCurrentFile():";

    // TODO check the returnValue!
    returnValue = moveCurrentFileToGivenSubfolder(c_hardcodedOutput);

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::trashCurrentFile() {
    bool returnValue(false);
    qDebug() << "CullendulaFileSystemHandler::trashCurrentFile():";

    // TODO check the returnValue!
    returnValue = moveCurrentFileToGivenSubfolder(c_hardcodedTrash);

    return returnValue;
}

//----------------------------------------------------------------------------

QString CullendulaFileSystemHandler::getCurrentStatus() const {
    auto returnValue = QString("showing %1 of %2")
                           .arg(QString::number(m_positionCurrentFile + 1),  // for the regular users indexing starts at 1 ..
                                QString::number(m_currentImages.size()));

    return returnValue;
}

//----------------------------------------------------------------------------

QString CullendulaFileSystemHandler::getLastErrorMessage() const { return m_lastErrorMessage; }

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::canUndo() { return m_undoStack.canUndo(); }

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::canRedo() { return m_undoStack.canRedo(); }

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::undo() {
    clearLastErrorMessage();
    if (!canUndo()) {
        setLastErrorMessage("No undo step is currently available.");
        return false;
    }

    qDebug() << "CullendulaFileSystemHandler::undo()";
    CullendulaUndoItem const item = m_undoStack.peekUndo();
    QString const targetPath = item.sourcePath;
    QString const sourcePath = item.targetPath;
    qDebug() << "\tsource: " << sourcePath;
    qDebug() << "\ttarget: " << targetPath;

    QDir fileHandler;
    bool const successfullyRenamed = fileHandler.rename(sourcePath, targetPath);
    qDebug() << "rename was: " << (successfullyRenamed ? "TRUE" : "ERROR");

    if (!successfullyRenamed) {
        setLastErrorMessage(formatMoveErrorMessage(QFileInfo(sourcePath).fileName(), "undo", "the filesystem rename operation failed"));
        return false;
    }

    m_undoStack.commitUndo();
    return rebuildImageFileList(targetPath, m_positionCurrentFile);
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::redo() {
    clearLastErrorMessage();
    if (!canRedo()) {
        setLastErrorMessage("No redo step is currently available.");
        return false;
    }

    qDebug() << "CullendulaFileSystemHandler::redo()";
    CullendulaUndoItem const item = m_undoStack.peekRedo();
    QString const targetPath = item.sourcePath;
    QString const sourcePath = item.targetPath;
    qDebug() << "\tsource: " << sourcePath;
    qDebug() << "\ttarget: " << targetPath;

    int fallbackPosition = m_positionCurrentFile;
    int const sourceIndex = findImageIndexByPath(sourcePath);
    if (sourceIndex >= 0) {
        fallbackPosition = sourceIndex;
    }

    QDir fileHandler;
    bool const successfullyRenamed = fileHandler.rename(sourcePath, targetPath);
    qDebug() << "rename was: " << (successfullyRenamed ? "TRUE" : "ERROR");

    if (!successfullyRenamed) {
        setLastErrorMessage(formatMoveErrorMessage(QFileInfo(sourcePath).fileName(), "redo", "the filesystem rename operation failed"));
        return false;
    }

    m_undoStack.commitRedo();
    return rebuildImageFileList(QString(), fallbackPosition);
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::processNewPath() {
    bool returnValue(false);

    qDebug() << "CullendulaFileSystemHandler::processNewPath():";

    // convert the given path (which maybe includes a filename)
    //    qDebug() << "fileInfo gets the following path:" << m_workingPath.path();

    // problem: windows shows as /c:/dir/file.suffix - linux here as /home/dir/file.suffix
    // so cut the first character for win, but don't for linux ..
    QString const intermediatePath =
#ifdef __linux__
        m_workingPath.path();
#else
        m_workingPath.path().remove(0, 1);  // remove the leading slash ("/")
#endif
    QFileInfo const fileInfo(intermediatePath);

    //! trim the path
    //! shall return for \Cullendula\testItemFolder --> \Cullendula\testItemFolder
    //! and for \Cullendula\testItemFolder\cat0.jpg --> \Cullendula\testItemFolder
    //    qDebug() << "\tfileInfo.isFile():" << fileInfo.isFile();
    //    qDebug() << "fileInfo.path(): " << fileInfo.path();
    //    qDebug() << "fileInfo.filePath(): " << fileInfo.filePath();
    //    qDebug() << "fileInfo.canonicalFilePath(): " << fileInfo.canonicalFilePath();
    qDebug() << "\tfileInfo.isDir():" << fileInfo.isDir();
    qDebug() << "fileInfo.absoluteFilePath(): " << fileInfo.absoluteFilePath();
    qDebug() << "fileInfo.absolutePath(): " << fileInfo.absolutePath();

    //    qDebug() << "fileInfo.fileName(): " << fileInfo.fileName();
    QDir const tempDir = QDir(fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.absolutePath());
    qDebug() << "\t resulting directory:" << tempDir.path();

    // additionally check if the directory is usable
    if (tempDir.exists()) {
        m_workingPath.setPath(tempDir.path());

        // trigger now the creation of the parsing
        returnValue = createImageFileList();

        // create now the output-folders
        returnValue &= createOutputFolder(c_hardcodedOutput);
        returnValue &= createOutputFolder(c_hardcodedTrash);
    } else {
        qDebug() << "ERROR: given directory does not exist";
    }

    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::createImageFileList() {
    qDebug() << "CullendulaFileSystemHandler::createImageFileList():";
    return rebuildImageFileList(QString(), 0);
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::rebuildImageFileList(QString const& preferredImagePath, int const fallbackPosition) {
    m_currentImages.clear();
    m_positionCurrentFile = -1;

    QSet<QString> const supportedImageSuffixes = getSupportedImageSuffixes();
    QFileInfoList const availableFiles = m_workingPath.entryInfoList(QDir::Files, QDir::Name);
    QFileInfoList availableImages;

    for (QFileInfo const& file : availableFiles) {
        QString const suffix = file.suffix().toLower();
        if (supportedImageSuffixes.contains(suffix) && m_allowedImageExtensions.contains(suffix)) {
            availableImages.append(file);
        }
    }

    if (availableImages.isEmpty()) {
        qDebug() << "no nice files found :(";
        return false;
    }

    qDebug() << "found the following image files:";
    for (QFileInfo const& file : availableImages) {
        qDebug() << "\t" << file.absoluteFilePath();
    }

    m_currentImages = availableImages.toVector();

    int const lastValidIndex = static_cast<int>(m_currentImages.size()) - 1;
    int positionToSelect = 0;
    if (!preferredImagePath.isEmpty()) {
        int const preferredIndex = findImageIndexByPath(preferredImagePath);
        if (preferredIndex >= 0) {
            positionToSelect = preferredIndex;
        } else {
            positionToSelect = std::clamp(fallbackPosition, 0, lastValidIndex);
        }
    } else {
        positionToSelect = std::clamp(fallbackPosition, 0, lastValidIndex);
    }

    m_positionCurrentFile = positionToSelect;
    return true;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::createOutputFolder(QString const& subdir) {
    bool returnValue(false);

    qDebug() << "CullendulaFileSystemHandler::createOutputFolder():" << subdir;

    QDir outputDirTest(m_workingPath.path() + QDir::separator() + subdir);

    if (outputDirTest.exists()) {
        qDebug() << "output-folder exists already :) - nothing to do";
        returnValue = true;
    } else {
        // create a new output dir
        bool const creationSuccessful = m_workingPath.mkdir(subdir);
        Q_UNUSED(creationSuccessful)  // return-value is not evaluated, because the next check test directly for existence of the directory

        if (outputDirTest.exists()) {
            qDebug() << "output-folder exists after creation!";
            returnValue = true;
        } else {
            qDebug() << "very severe error - could not create output-dir :(";
        }
    }

    // TODO maybe return something or throw or whatever ... delete /home/..
    return returnValue;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::moveCurrentFileToGivenSubfolder(QString const& subdir) {
    bool returnValue(false);
    qDebug() << "CullendulaFileSystemHandler::moveCurrentFileToGivenSubfolder():";
    clearLastErrorMessage();

    bool const saneInternalState = checkInternalSanity();
    if (!saneInternalState) {
        setLastErrorMessage("No current image is available to move.");
        return false;
    }

    QDir outputDir(m_workingPath.path() + QDir::separator() + subdir);
    if (!outputDir.exists()) {
        setLastErrorMessage(QString("Target directory '%1' is unavailable.").arg(outputDir.path()));
        return false;
    }

    QString const sourcePathAndName(m_currentImages[m_positionCurrentFile].absoluteFilePath());
    qDebug() << "\t sourcePathAndName:" << sourcePathAndName;
    QFileInfo const fileInfo(sourcePathAndName);
    QString const fileName(fileInfo.fileName());
    QString const preferredTargetPath = outputDir.path() + QDir::separator() + fileName;
    QString const targetPathAndName = createUniqueTargetPath(preferredTargetPath);
    qDebug() << "\t targetPathAndName:" << targetPathAndName;
    bool const successfullyRenamed = outputDir.rename(sourcePathAndName, targetPathAndName);  //! this is the MOVE operation!
    qDebug() << "\t successfullyRenamed?" << successfullyRenamed;

    if (!successfullyRenamed) {
        setLastErrorMessage(formatMoveErrorMessage(fileName, subdir, "the filesystem rename operation failed"));
        return false;
    }

    //! add to the stack for possible undoing
    m_undoStack.push(sourcePathAndName, targetPathAndName);

    // go to the next picture by removing the entry from the file-list, but keep the position
    m_currentImages.removeAt(m_positionCurrentFile);
    // if this was the last item of the list (like pos 2 at list of 3; which has now just 2 elements), then modulo
    int const listSize = m_currentImages.size();
    m_positionCurrentFile = (listSize > 0) ? (m_positionCurrentFile % listSize) : -1;
    returnValue = true;

    return returnValue;
}

//----------------------------------------------------------------------------

int CullendulaFileSystemHandler::findImageIndexByPath(QString const& imagePath) const {
    for (int index = 0; index < m_currentImages.size(); ++index) {
        if (m_currentImages[index].absoluteFilePath() == imagePath) {
            return index;
        }
    }

    return -1;
}

//----------------------------------------------------------------------------

QString CullendulaFileSystemHandler::createUniqueTargetPath(QString const& initialTargetPath) const {
    if (!QFileInfo::exists(initialTargetPath)) {
        return initialTargetPath;
    }

    QFileInfo const fileInfo(initialTargetPath);
    QString const directoryPath = fileInfo.absolutePath();
    QString const completeBaseName = fileInfo.completeBaseName();
    QString const completeSuffix = fileInfo.completeSuffix();
    QString const suffixPrefix = completeSuffix.isEmpty() ? QString() : "." + completeSuffix;

    for (int counter = 1;; ++counter) {
        QString const candidateFileName = QString("%1 (%2)%3").arg(completeBaseName, QString::number(counter), suffixPrefix);
        QString const candidatePath = directoryPath + QDir::separator() + candidateFileName;
        if (!QFileInfo::exists(candidatePath)) {
            return candidatePath;
        }
    }
}

//----------------------------------------------------------------------------

void CullendulaFileSystemHandler::clearLastErrorMessage() { m_lastErrorMessage.clear(); }

//----------------------------------------------------------------------------

void CullendulaFileSystemHandler::setLastErrorMessage(QString const& message) { m_lastErrorMessage = message; }

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::checkInternalSanity() const {
    bool returnValue(true);

    QDir const outputDir(m_workingPath.path());
    if (!outputDir.exists()) {
        qDebug() << "CullendulaFileSystemHandler::checkInternalSanity(): ERROR: workingPath not valid!";
        returnValue = false;
    }

    if (m_currentImages.empty()) {
        qDebug() << "CullendulaFileSystemHandler::checkInternalSanity(): ERROR: list of current images is empty!";
        returnValue = false;
    }

    if (m_positionCurrentFile < 0) {
        qDebug() << "CullendulaFileSystemHandler::checkInternalSanity(): ERROR: position" << QString::number(m_positionCurrentFile) << "is a negative number!";
        returnValue = false;
    }

    if (m_positionCurrentFile >= m_currentImages.size()) {
        qDebug() << "CullendulaFileSystemHandler::checkInternalSanity(): ERROR: position" << QString::number(m_positionCurrentFile)
                 << " is out of range of the image-list with size" << QString::number(m_currentImages.size());
        returnValue = false;
    }

    return returnValue;
}
