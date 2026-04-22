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

QStringList normalizeExtensions(QStringList const& extensions, QSet<QString> const& supportedSuffixes) {
    QStringList normalizedExtensions;

    for (QString const& extension : extensions) {
        QString const normalizedExtension = extension.trimmed().toLower();
        if (!normalizedExtension.isEmpty() && supportedSuffixes.contains(normalizedExtension) && !normalizedExtensions.contains(normalizedExtension)) {
            normalizedExtensions.append(normalizedExtension);
        }
    }

    return normalizedExtensions;
}

QStringList normalizeExtensions(QStringList const& extensions) { return normalizeExtensions(extensions, getSupportedImageSuffixes()); }

QString formatMoveErrorMessage(QString const& fileName, QString const& subdir, QString const& details) {
    //: Error message after moving a file into a named subdirectory such as "output", "trash", "undo", or "redo" failed.
    return CullendulaFileSystemHandler::tr("Could not move '%1' to '%2': %3").arg(fileName, subdir, details);
}

QString formatDirectorySetupErrorMessage(QString const& subdir, QString const& directoryPath, QString const& details) {
    //: Error message after preparing an application-managed subdirectory such as "output" or "trash" failed.
    return CullendulaFileSystemHandler::tr("Could not prepare '%1' directory at '%2': %3").arg(subdir, directoryPath, details);
}

bool pathExists(QString const& path) { return QFileInfo(path).exists(); }

bool pathIsDirectory(QString const& path) { return QFileInfo(path).isDir(); }

bool directoryExists(QString const& path) { return QDir(path).exists(); }

bool mkdirInDirectory(QDir& parentDir, QString const& subdir) { return parentDir.mkdir(subdir); }
}  // namespace

//----------------------------------------------------------------------------

CullendulaFileSystemHandler::CullendulaFileSystemHandler() : m_workingPath("") {
    setAllowedImageExtensions(getSuggestedImageExtensions());
    qDebug() << "CullendulaFileSystemHandler::CullendulaFileSystemHandler()";
}

//----------------------------------------------------------------------------

QStringList CullendulaFileSystemHandler::getSuggestedImageExtensions() {
    return CullendulaFileSystemHandlerDetail::getSuggestedImageExtensions(getSupportedImageSuffixes());
}

//----------------------------------------------------------------------------

QStringList CullendulaFileSystemHandlerDetail::getSuggestedImageExtensions(QSet<QString> const& supportedSuffixes) {
    QStringList const preferredExtensions = {"png", "jpg", "jpeg", "webp", "gif", "bmp", "tif", "tiff", "svg", "ico"};

    QStringList suggestedExtensions = normalizeExtensions(preferredExtensions, supportedSuffixes);
    if (suggestedExtensions.size() >= c_maxSuggestedExtensions) {
        suggestedExtensions.resize(c_maxSuggestedExtensions);
        return suggestedExtensions;
    }

    QStringList additionalExtensions = supportedSuffixes.values();
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

CullendulaFileSystemHandlerDetail::OutputFolderHooks CullendulaFileSystemHandlerDetail::defaultOutputFolderHooks() {
    return OutputFolderHooks{pathExists, pathIsDirectory, directoryExists, mkdirInDirectory};
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandlerDetail::createOutputFolder(QDir& workingPath, QString const& subdir, QString& errorMessage, OutputFolderHooks const& hooks) {
    QString const outputDirPath = workingPath.path() + QDir::separator() + subdir;

    if (hooks.pathExists(outputDirPath) && !hooks.pathIsDirectory(outputDirPath)) {
        errorMessage = formatDirectorySetupErrorMessage(subdir, outputDirPath,
                                                        CullendulaFileSystemHandler::tr("the path is already occupied by a non-directory filesystem entry"));
        return false;
    }

    if (hooks.directoryExists(outputDirPath)) {
        qDebug() << "output-folder exists already :) - nothing to do";
        return true;
    }

    bool const creationSuccessful = hooks.mkdir(workingPath, subdir);
    if (!creationSuccessful) {
        errorMessage = formatDirectorySetupErrorMessage(subdir, outputDirPath, CullendulaFileSystemHandler::tr("creating the directory failed"));
        return false;
    }

    if (!hooks.directoryExists(outputDirPath)) {
        qDebug() << "very severe error - could not create output-dir :(";
        errorMessage =
            formatDirectorySetupErrorMessage(subdir, outputDirPath, CullendulaFileSystemHandler::tr("the directory is still missing after creation"));
        return false;
    }

    qDebug() << "output-folder exists after creation!";
    return true;
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

    if (checkInternalSanity()) {
        QString const path(m_currentImages[m_positionCurrentFile].absoluteFilePath());
        qDebug() << "\tpath:" << path;
        QFile const tempFile(path);
        if (tempFile.exists()) {
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
    qDebug() << "CullendulaFileSystemHandler::saveCurrentFile():";
    clearLastErrorMessage();

    return moveCurrentFileToGivenSubfolder(c_hardcodedOutput);
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::trashCurrentFile() {
    qDebug() << "CullendulaFileSystemHandler::trashCurrentFile():";
    clearLastErrorMessage();

    return moveCurrentFileToGivenSubfolder(c_hardcodedTrash);
}

//----------------------------------------------------------------------------

QString CullendulaFileSystemHandler::getCurrentStatus() const {
    //: Status bar message showing the current 1-based image position and the total number of loaded images.
    auto returnValue = tr("showing %1 of %2").arg(QString::number(m_positionCurrentFile + 1), QString::number(m_currentImages.size()));

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
        //: Error shown when the user requests Undo but the undo history is empty.
        setLastErrorMessage(tr("No undo step is currently available."));
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
        //: Low-level filesystem failure detail inserted into a larger user-visible move error message.
        setLastErrorMessage(formatMoveErrorMessage(QFileInfo(sourcePath).fileName(), QStringLiteral("undo"), tr("the filesystem rename operation failed")));
        return false;
    }

    m_undoStack.commitUndo();
    return rebuildImageFileList(targetPath, m_positionCurrentFile);
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::redo() {
    clearLastErrorMessage();
    if (!canRedo()) {
        //: Error shown when the user requests Redo but the redo history is empty.
        setLastErrorMessage(tr("No redo step is currently available."));
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
        //: Low-level filesystem failure detail inserted into a larger user-visible move error message.
        setLastErrorMessage(formatMoveErrorMessage(QFileInfo(sourcePath).fileName(), QStringLiteral("redo"), tr("the filesystem rename operation failed")));
        return false;
    }

    m_undoStack.commitRedo();
    return rebuildImageFileList(QString(), fallbackPosition);
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::processNewPath() {
    bool returnValue(false);

    qDebug() << "CullendulaFileSystemHandler::processNewPath():";

    // Dropped file URLs can carry a leading slash before the Windows drive letter.
    QString const intermediatePath =
#ifdef __linux__
        m_workingPath.path();
#else
        m_workingPath.path().remove(0, 1);  // remove the leading slash ("/")
#endif
    QFileInfo const fileInfo(intermediatePath);

    qDebug() << "\tfileInfo.isDir():" << fileInfo.isDir();
    qDebug() << "fileInfo.absoluteFilePath(): " << fileInfo.absoluteFilePath();
    qDebug() << "fileInfo.absolutePath(): " << fileInfo.absolutePath();

    QDir const tempDir = QDir(fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.absolutePath());
    qDebug() << "\t resulting directory:" << tempDir.path();

    // additionally check if the directory is usable
    if (tempDir.exists()) {
        m_workingPath.setPath(tempDir.path());

        bool const foundImages = createImageFileList();
        bool const outputReady = createOutputFolder(c_hardcodedOutput);
        bool const trashReady = createOutputFolder(c_hardcodedTrash);

        returnValue = foundImages && outputReady && trashReady;

        if (!outputReady || !trashReady) {
            m_currentImages.clear();
            m_positionCurrentFile = -1;
            m_undoStack = CullendulaUndoStack();
        }
    } else {
        qDebug() << "ERROR: given directory does not exist";
        //: Error after a dropped path or selected path does not resolve to an existing directory on disk.
        setLastErrorMessage(tr("The path '%1' could not be resolved to an existing directory.").arg(intermediatePath));
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
    qDebug() << "CullendulaFileSystemHandler::createOutputFolder():" << subdir;
    QString errorMessage;
    bool const success = CullendulaFileSystemHandlerDetail::createOutputFolder(m_workingPath, subdir, errorMessage,
                                                                               CullendulaFileSystemHandlerDetail::defaultOutputFolderHooks());
    if (!success) {
        setLastErrorMessage(errorMessage);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------

bool CullendulaFileSystemHandler::moveCurrentFileToGivenSubfolder(QString const& subdir) {
    bool returnValue(false);
    qDebug() << "CullendulaFileSystemHandler::moveCurrentFileToGivenSubfolder():";
    clearLastErrorMessage();

    bool const saneInternalState = checkInternalSanity();
    if (!saneInternalState) {
        //: Error shown when the user tries to save or trash an image but no current image is loaded.
        setLastErrorMessage(tr("No current image is available to move."));
        return false;
    }

    if (!createOutputFolder(subdir)) {
        return false;
    }

    QDir outputDir(m_workingPath.path() + QDir::separator() + subdir);

    QString const sourcePathAndName(m_currentImages[m_positionCurrentFile].absoluteFilePath());
    qDebug() << "\t sourcePathAndName:" << sourcePathAndName;
    QFileInfo const fileInfo(sourcePathAndName);
    QString const fileName(fileInfo.fileName());
    QString const preferredTargetPath = outputDir.path() + QDir::separator() + fileName;
    QString const targetPathAndName = createUniqueTargetPath(preferredTargetPath);
    qDebug() << "\t targetPathAndName:" << targetPathAndName;
    bool const successfullyRenamed = outputDir.rename(sourcePathAndName, targetPathAndName);
    qDebug() << "\t successfullyRenamed?" << successfullyRenamed;

    if (!successfullyRenamed) {
        //: Low-level filesystem failure detail inserted into a larger user-visible move error message.
        setLastErrorMessage(formatMoveErrorMessage(fileName, subdir, tr("the filesystem rename operation failed")));
        return false;
    }

    m_undoStack.push(sourcePathAndName, targetPathAndName);

    m_currentImages.removeAt(m_positionCurrentFile);
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
