# Worst Findings

This list is ordered by engineering risk, with emphasis on user-data safety, correctness, test signal, and maintainability.

## 1. Undo/redo does not restore in-memory application state after moving files

- References: `src/CullendulaFileSystemHandler.cpp:157`, `src/CullendulaFileSystemHandler.cpp:185`
- Problem: `undo()` and `redo()` rename files on disk, but they never rebuild `m_currentImages` or repair `m_positionCurrentFile`. The code even contains TODO comments acknowledging the missing rescan.
- Impact: after an undo/redo, the app's on-disk state and in-memory state diverge. The UI can point at the wrong file set, navigation can become inconsistent, and subsequent save/trash operations act on stale assumptions.

## 2. Undo/redo UI actions do not refresh the main view after state changes

- References: `src/CullendulaMainWindow.cpp:296`, `src/CullendulaMainWindow.cpp:301`, `src/CullendulaMainWindow.cpp:236`
- Problem: the menu actions call `m_fileSystemHandler.undo()/redo()` and then only update the enablement of the undo/redo actions. They never call `refreshLabel()` or otherwise re-sync the visible image and the four main buttons.
- Impact: even if filesystem undo/redo were fixed, the UI would still present stale state after those actions. Users would not have a trustworthy view of what the app believes is current.

## 3. File move failures are not surfaced to the user and there is no name-collision strategy

- References: `src/CullendulaFileSystemHandler.cpp:343`, `src/CullendulaMainWindow.cpp:169`, `src/CullendulaMainWindow.cpp:189`
- Problem: moving a file relies on `QDir::rename()` into `output/` or `trash/`. If the target path already exists, or the rename otherwise fails, the code only emits debug logging. The GUI shows no actionable error and offers no collision handling.
- Impact: a photo culling tool is a data-management tool. Silent failure around moves is high-risk because users may believe a file was classified when it was not, especially across large sessions with repeated filenames.

## 4. Drag-and-drop accepts everything on entry and validates too late

- References: `src/CullendulaMainWindow.cpp:79`, `src/CullendulaMainWindow.cpp:88`
- Problem: `dragEnterEvent()` unconditionally accepts the drag, even though `dropEvent()` only handles URL payloads. The app advertises acceptance before it knows whether the payload is usable.
- Impact: this produces misleading UX and increases the chance of odd states or confusing no-op drops. Input validation should happen at the boundary, not after the UI already signaled acceptance.

## 5. Image loading and scaling is fully synchronous on every resize event

- References: `src/CullendulaMainWindow.cpp:124`, `src/CullendulaMainWindow.cpp:204`
- Problem: every window resize triggers `refreshLabel()`, which reloads the image from disk and rescales it immediately on the UI thread.
- Impact: with large JPEGs, resize interactions can become visibly laggy. The cost compounds because the code decodes from disk repeatedly instead of caching the original image or deferring expensive work.
