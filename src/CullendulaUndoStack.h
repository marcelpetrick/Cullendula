//----------------------------------------------------------------------------------
// description: Cullendula - small GUI-app to pick the best shots from a session
// author: mail@marcelpetrick.it
// repo: https://github.com/marcelpetrick/Cullendula
//----------------------------------------------------------------------------------

// due to concerns while thinking about the API in combination with Redo, reading about the Command/Memento-Pattern is necessary ..
// https://en.wikipedia.org/wiki/Command_pattern
// https://en.wikipedia.org/wiki/Memento_pattern

#pragma once

// Qt includes
#include <QtCore/QString>
#include <QtCore/QVector>

//----------------------------------------------------------------------------------

/*!
 * @file
 * @brief Undo/redo data structures for file move operations inside Cullendula.
 */

/*!
 * @brief Value object that describes one file move operation.
 *
 * The application models undo and redo as file renames between two absolute paths.
 * Each item therefore stores the original location and the destination location of
 * a single move so that the caller can replay the operation in either direction.
 */
class CullendulaUndoItem {
   public:
    //! Construct an empty item.
    CullendulaUndoItem() = default;
    //! Destroy the item.
    ~CullendulaUndoItem() = default;

    /*!
     * @brief Construct an item from a source and target path.
     * @param from Absolute path the file is moved from.
     * @param to Absolute path the file is moved to.
     */
    CullendulaUndoItem(QString const& from, QString const& to) : sourcePath(from), targetPath(to) {
        // nothing else to do :)
    }

    /*!
     * @brief Absolute source path of the move operation.
     *
     * For an undo item pushed by the file system handler this is the path before
     * the file was moved.
     */
    QString sourcePath;

    /*!
     * @brief Absolute destination path of the move operation.
     *
     * For an undo item pushed by the file system handler this is the path after
     * the file was moved.
     */
    QString targetPath;
};

//----------------------------------------------------------------------------------

/*!
 * @brief Lightweight undo/redo stack for file move operations.
 *
 * The stack stores move operations as pairs of absolute paths. Calling undo()
 * returns the most recent move and transfers the inverse operation to the redo
 * stack. Calling redo() performs the symmetrical transfer back to the undo
 * stack. The class itself only manages history; callers remain responsible for
 * actually renaming files on disk.
 */
class CullendulaUndoStack {
   public:
    //! Construct an empty undo stack.
    CullendulaUndoStack() = default;

    //! Destroy the undo stack.
    ~CullendulaUndoStack() = default;

    /*!
     * @brief Append a new move operation to the undo history.
     * @param from Absolute source path before the move.
     * @param to Absolute destination path after the move.
     *
     * Pushing a new operation clears any redo history because it starts a new
     * branch of user actions.
     */
    void push(QString const& from, QString const& to);

    /*!
     * @brief Pop the most recent undo item and prepare the matching redo entry.
     * @return The last operation from the undo history, or an empty default item
     *         when no undo step is available.
     */
    CullendulaUndoItem undo();

    /*!
     * @brief Pop the most recent redo item and prepare the matching undo entry.
     * @return The last operation from the redo history, or an empty default item
     *         when no redo step is available.
     */
    CullendulaUndoItem redo();

    /*!
     * @brief Check whether at least one undo step is available.
     * @return `true` when the undo history is non-empty.
     */
    bool canUndo();

    /*!
     * @brief Check whether at least one redo step is available.
     * @return `true` when the redo history is non-empty.
     */
    bool canRedo();

    /*!
     * @brief Return the current number of undo entries.
     * @return Depth of the undo stack.
     */
    long getUndoDepth();

    /*!
     * @brief Return the current number of redo entries.
     * @return Depth of the redo stack.
     */
    long getRedoDepth();

   private:
    //! Storage for operations that can currently be undone.
    QVector<CullendulaUndoItem> m_undoContainer;

    //! Storage for operations that can currently be redone.
    QVector<CullendulaUndoItem> m_redoContainer;
};
