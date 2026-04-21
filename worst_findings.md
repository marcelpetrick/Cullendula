# Worst Findings

This list is ordered by engineering risk, with emphasis on user-data safety, correctness, test signal, and maintainability.

## 1. Loading a new path destroys the current session before the new path is validated

- References: `src/CullendulaFileSystemHandler.cpp:106`, `src/CullendulaFileSystemHandler.cpp:122`, `src/CullendulaFileSystemHandler.cpp:282`, `src/CullendulaMainWindow.cpp:245`
- Problem: `setWorkingPath()` resets the current image list, selection, undo stack, and last error before the newly dropped path has been proven valid. A bad drop or unusable folder therefore discards the existing session first and only reports failure afterwards.
- Impact: a user can lose undo history and the active review session because of one invalid drag-and-drop input. Senior engineering instinct is to stage the new state and only commit it after validation succeeds.

## 2. The pipeline is not a pure verification step; it rewrites source files and still exits successfully

- References: `localPipeline.sh:448`, `localPipeline.sh:475`, `localPipeline.sh:493`, `localPipeline.sh:678`, `localPipeline.sh:691`
- Problem: the pipeline runs `clang-format -i`, which mutates tracked files during validation, and it reports `WARN` rather than failure when formatting drift is found. The final exit condition checks only whether the formatting step ran, not whether it changed files.
- Impact: CI-style tooling that rewrites the repository and still returns success is a bad contract. It hides drift, makes runs non-reproducible, and creates exactly the kind of “pipeline passed but the worktree changed” confusion that should never survive senior review.

## 3. Drag-and-drop path handling is brittle and manually platform-specific

- References: `src/CullendulaMainWindow.cpp:246`, `src/CullendulaFileSystemHandler.cpp:290`
- Problem: the drop path is taken from `QUrl::path()` and then manually massaged with `#ifdef __linux__` plus `remove(0, 1)` for other platforms. That is not the robust Qt way to handle local file URLs.
- Impact: this is fragile around Windows drive letters, UNC paths, encoded characters, and other platform-specific path shapes. Correct path handling should use `QUrl::toLocalFile()` and avoid ad hoc string surgery in core filesystem code.

## 4. A missing current file leaves the main window in an inconsistent UI state

- References: `src/CullendulaMainWindow.cpp:389`, `src/CullendulaMainWindow.cpp:420`
- Problem: when `refreshLabel()` sees a non-empty path that no longer exists on disk, it only clears the cached pixmap and logs a debug message. It does not update the label text, disable buttons, or show a status-bar error.
- Impact: the user can be left with stale controls that still look actionable even though the current file is gone. That is a correctness and UX boundary issue in a file-management application.

## 5. Test registration is monolithic, so CTest signal and parallelism are much weaker than they look

- References: `tests/CMakeLists.txt:41`, `tests/main.cpp:22`
- Problem: the entire test program is registered as one CTest case, and all suites are multiplexed manually through a custom `main.cpp`. CTest therefore sees only one coarse-grained test, even though the pipeline asks it to run with parallel workers.
- Impact: failure isolation is poor, reporting is coarse, retries are inefficient, and real test parallelism is unavailable. A senior engineer would usually split suites into separate CTest test cases or executables so the tooling can do its job.

## 6. Release/version information is duplicated across several manual edit surfaces

- References: `CMakeLists.txt:7`, `src/CullendulaMainWindow.cpp:34`, `tests/Test_CullendulaMainWindow.cpp:160`, `README.md:166`
- Problem: every release requires synchronized manual edits in build metadata, UI strings, tests, and documentation. There is no single source of truth for the version.
- Impact: this creates unnecessary release churn and an avoidable inconsistency risk. Senior engineers usually centralize version metadata and generate or consume it everywhere else.

## 7. The production bootstrap forces `QT_QPA_PLATFORM=offscreen` when the environment is empty

- References: `src/main.cpp:16`, `src/CullendulaAppBootstrap.cpp:15`
- Problem: the same helper used for tests is also used by the production executable, and it silently forces the app into offscreen mode whenever the environment variable is unset.
- Impact: that can mask real platform/plugin configuration issues and may produce an application that “runs” but is not actually visible to the user. Test-only environment shaping should not bleed into production startup without a clear opt-in.

## 8. The bootstrap tests mutate process-global environment state and do not restore it

- References: `tests/Test_CullendulaAppBootstrap.cpp:14`, `tests/Test_CullendulaAppBootstrap.cpp:24`
- Problem: the tests directly call `qunsetenv()` and `qputenv()` on `QT_QPA_PLATFORM` but never restore the previous value.
- Impact: this makes the test process order-dependent and contaminates later suites with hidden global state. That is the kind of subtle test pollution that keeps a suite “green until it isn’t”.

## 9. The project still treats branch coverage as informational despite it being the obvious weak spot

- References: `localPipeline.sh:384`, `localPipeline.sh:633`, `README.md:162`
- Problem: the pipeline gates only total line coverage, while branch coverage remains around the 50% range and is not enforced at all. The tooling prints branch data, but the project process effectively ignores it.
- Impact: the current quality signal overstates how thoroughly control-flow and failure paths are exercised. For this codebase, the remaining engineering risk is much more concentrated in branches than in raw line execution.

## 10. The codebase still leans heavily on ad hoc debug logging and TODO comments instead of structured error handling

- References: `src/CullendulaFileSystemHandler.cpp:184`, `src/CullendulaFileSystemHandler.cpp:196`, `src/CullendulaFileSystemHandler.cpp:410`, `src/CullendulaMainWindow.cpp:13`
- Problem: core logic still contains TODO-style comments around return-value handling, many `qDebug()` diagnostics, and no clear policy for logging versus user-facing error propagation.
- Impact: this makes operational behavior harder to reason about and leaves important failure semantics implicit in comments instead of enforced in code. It is the kind of engineering looseness that compounds over time.
