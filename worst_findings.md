# Worst Findings

This list is ordered by engineering risk, with emphasis on user-data safety, correctness, test signal, and maintainability.

## 1. Undo/redo does not restore in-memory application state after moving files

- References: `src/CullendulaFileSystemHandler.cpp:157`, `src/CullendulaFileSystemHandler.cpp:185`
- Problem: `undo()` and `redo()` rename files on disk, but they never rebuild `m_currentImages` or repair `m_positionCurrentFile`. The code even contains TODO comments acknowledging the missing rescan.
- Impact: after an undo/redo, the app's on-disk state and in-memory state diverge. The UI can point at the wrong file set, navigation can become inconsistent, and subsequent save/trash operations act on stale assumptions.

## 2. Changing to a new folder does not reset prior session state

- References: `src/CullendulaFileSystemHandler.cpp:35`, `src/CullendulaFileSystemHandler.cpp:213`, `src/CullendulaFileSystemHandler.cpp:269`, `src/CullendulaFileSystemHandler.h:82`
- Problem: `setWorkingPath()` updates `m_workingPath`, but it does not clear `m_currentImages`, `m_positionCurrentFile`, or the undo stack before processing the new path. `createImageFileList()` only assigns a new list when images are found; otherwise it leaves the previous list intact.
- Impact: dropping an empty or invalid folder can leave the app showing and operating on the previous folder's images while the working path points somewhere else. That is a serious correctness problem for a tool that moves files.
- Status: addressed. `setWorkingPath()` now resets handler state before processing the new path, `createImageFileList()` clears the in-memory list before rescanning, and the test suite covers both handler-level reload failure and the GUI case of dropping an empty folder after a populated one.

## 3. Redo history is never invalidated after a new action

- References: `src/CullendulaUndoStack.cpp:12`
- Problem: `CullendulaUndoStack::push()` appends to the undo container but never clears `m_redoContainer`.
- Impact: once the user undoes an action and then performs a new save/trash move, redo history should be discarded. Keeping stale redo entries violates standard undo/redo semantics and can replay operations from the wrong branch of history.
- Status: addressed. `CullendulaUndoStack::push()` now clears redo history before appending the new undo item.

## 4. The repository's test suite is currently failing

- References: `tests/Test_CullendulaUndoStack.cpp:84`, `tests/Test_CullendulaUndoStack.cpp:134`, `tests/Test_CullendulaUndoStack.cpp:159`, `README.md:26`
- Problem: the checked-in tests do not pass under the current build. `/usr/bin/ctest --output-on-failure` reports 3 failures in the only registered test target.
- Impact: the project has no trustworthy regression gate at the moment. That removes the main safety net for changing core behavior and contradicts the README's claim that the tests pass.
- Status: addressed. The unit-test target now passes again from the CLI and the README documents the working test commands.

## 5. The tests themselves assert the wrong fields, so they do not reliably specify undo semantics

- References: `tests/Test_CullendulaUndoStack.cpp:76`, `tests/Test_CullendulaUndoStack.cpp:83`, `tests/Test_CullendulaUndoStack.cpp:87`, `tests/Test_CullendulaUndoStack.cpp:95`, `tests/Test_CullendulaUndoStack.cpp:98`, `tests/Test_CullendulaUndoStack.cpp:101`, `tests/Test_CullendulaUndoStack.cpp:124`, `tests/Test_CullendulaUndoStack.cpp:133`
- Problem: multiple assertions compare `targetPath` twice and never verify `sourcePath`. Example: `(bar.targetPath == "a") && (bar.targetPath == "b")`.
- Impact: even if the suite were green, it would still be a weak signal because the assertions do not correctly model the data structure being tested. This is worse than just "missing tests"; it gives misleading confidence.
- Status: addressed. The tests now verify both `sourcePath` and `targetPath`, and they cover the redo-clearing behavior on new pushes.

## 6. Undo/redo UI actions do not refresh the main view after state changes

- References: `src/CullendulaMainWindow.cpp:296`, `src/CullendulaMainWindow.cpp:301`, `src/CullendulaMainWindow.cpp:236`
- Problem: the menu actions call `m_fileSystemHandler.undo()/redo()` and then only update the enablement of the undo/redo actions. They never call `refreshLabel()` or otherwise re-sync the visible image and the four main buttons.
- Impact: even if filesystem undo/redo were fixed, the UI would still present stale state after those actions. Users would not have a trustworthy view of what the app believes is current.

## 7. File move failures are not surfaced to the user and there is no name-collision strategy

- References: `src/CullendulaFileSystemHandler.cpp:343`, `src/CullendulaMainWindow.cpp:169`, `src/CullendulaMainWindow.cpp:189`
- Problem: moving a file relies on `QDir::rename()` into `output/` or `trash/`. If the target path already exists, or the rename otherwise fails, the code only emits debug logging. The GUI shows no actionable error and offers no collision handling.
- Impact: a photo culling tool is a data-management tool. Silent failure around moves is high-risk because users may believe a file was classified when it was not, especially across large sessions with repeated filenames.

## 8. Drag-and-drop accepts everything on entry and validates too late

- References: `src/CullendulaMainWindow.cpp:79`, `src/CullendulaMainWindow.cpp:88`
- Problem: `dragEnterEvent()` unconditionally accepts the drag, even though `dropEvent()` only handles URL payloads. The app advertises acceptance before it knows whether the payload is usable.
- Impact: this produces misleading UX and increases the chance of odd states or confusing no-op drops. Input validation should happen at the boundary, not after the UI already signaled acceptance.

## 9. Image loading and scaling is fully synchronous on every resize event

- References: `src/CullendulaMainWindow.cpp:124`, `src/CullendulaMainWindow.cpp:204`
- Problem: every window resize triggers `refreshLabel()`, which reloads the image from disk and rescales it immediately on the UI thread.
- Impact: with large JPEGs, resize interactions can become visibly laggy. The cost compounds because the code decodes from disk repeatedly instead of caching the original image or deferring expensive work.

## 10. The top-level CMake option for disabling tests is incomplete

- References: `CMakeLists.txt:20`, `CMakeLists.txt:24`, `CMakeLists.txt:35`
- Problem: the repo exposes `CULLENDULA_BUILD_TESTS`, but `find_package(Qt5 5.15 REQUIRED COMPONENTS ... Test)` is unconditional. A machine without `Qt5::Test` still cannot configure the project even if tests are disabled.
- Impact: this makes the advertised build switch misleading and needlessly increases the dependency surface for normal application builds.
- Status: addressed. `Qt5::Test` is now discovered only when `CULLENDULA_BUILD_TESTS` is enabled.
