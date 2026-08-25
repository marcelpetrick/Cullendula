# Changelog

All released versions of Cullendula, newest at the bottom.
Every commit bumps the patch version and appends one entry here; see [`AGENTS.md`](AGENTS.md) for the rule.

* v0.1 was the basic release; working, but ugly
* v0.2 improved useability and stability; more features (move to trash!); refactored code-base; improved code-quality
* v0.3 added tooltips; fixed the "pumping center-label"-issue; added menus; fixed some resizing-issues with the image-label
* v0.4 added undo/redo-functionality with unit-test; added a nice violet icon for the executable and program
* v0.5 moved the buildsystem to cmake (from qmake)
* v0.5.1 suppresses the QtCreator maintenance-tool warning during CMake configure
* v0.5.2 makes the CMake project buildable out of the box from QtCreator
* v0.5.3 fixes the QTest target integration for XML output
* v0.5.4 fixed the undo-stack unit tests, clarified the CLI test workflow, and corrected the README
* v0.5.5 adds opt-in coverage support with standard gcov-based tooling
* v0.5.6 expanded coverage with deterministic MainWindow tests and documented the HTML coverage workflow
* v0.6.0 ports the project build and test setup to Qt 6.10
* v0.6.1 restores image loading across the Qt-supported readable image formats
* v0.6.2 adds a configurable `Main -> Extensions` menu for choosing which image suffixes are loaded
* v0.6.3 adds a repository-local clang-format configuration based on the default Google C++ style
* v0.6.4 closes the stale-session reload gap with explicit regression coverage when switching to an empty folder
* v0.6.5 strengthens the test suite around extension-filter normalization and the all-filters-disabled UI case
* v0.6.6 adds switchable light and dark widget themes under `Main -> Style`
* v0.6.7 adds a Doxygen target with generated HTML output, warning logging, and dependency graphs
* v0.6.8 fixes the remaining Doxygen warnings and keeps the generated warning log clean
* v0.6.9 keeps undo/redo synchronized across the filesystem state, in-memory image list, and visible main view
* v0.6.10 applies the selected light or dark theme across the application palette and Qt dialogs
* v0.6.11 fixes the doxygen-documentation globally
* v0.6.12 adds a repository-local pipeline script for build, test, docs, coverage, and formatting checks
* v0.6.13 resolves filename collisions during save/trash moves and surfaces move failures to the user
* v0.6.14 validates drag payloads at drag-enter time so unsupported drops are rejected before the UI advertises acceptance
* v0.6.15 caches the currently displayed image so window resizes only rescale the in-memory preview instead of reloading from disk
* v0.6.16 broadens coverage across bootstrap, filesystem, main-window, and undo-stack edge cases to exercise more failure paths and branch outcomes
* v0.6.17 aligns the repository with clang-format output so the local pipeline finishes with a clean worktree after formatting checks
* v0.6.18 makes undo/redo history transitions atomic with the filesystem rename so failed undo or redo attempts preserve history and surface actionable errors
* v0.6.19 replaces the remaining filesystem TODOs with explicit directory-setup error handling, on-demand recreation of output folders, and regression coverage for those failure paths
* v0.6.20 extends the local pipeline with a final app-launch step that waits for the user to close Cullendula without changing the script exit status
* v0.6.21 lays the Qt 6 localization groundwork with embedded TS/QM resources, runtime `QTranslator` switching, and a new language menu for English, German, Croatian, and Chinese
* v0.6.22 adds the `localPipeline.sh --noRun` option so the final app launch can be skipped without affecting the script return value
* v0.6.23 makes CMake the single source of truth for the visible application version while keeping the version in the window title
* v0.6.24 marks the remaining user-visible strings for Qt translation extraction and documents the TS/QM workflow in the README
* v0.6.25 prepares the German, Croatian, and Chinese translations for the current Qt 6 localization scaffolding
* v0.6.26 adds translator-facing Qt context comments and UI string comments so Linguist translations can distinguish ambiguous labels and status text more reliably
* v0.6.27 adds a third gloomy purple-and-cyan style while keeping the existing light and dark themes intact
* v0.6.28 covers the real application entry point with a headless executable smoke test and includes `main.cpp` in coverage reporting
* v0.6.29 pushes bootstrap and filesystem coverage further with deterministic helper seams, stronger edge-case tests, and improved coverage of failure-path handling
* v0.6.30 adds repository-local Cppcheck infrastructure with compilation-database input, XML and HTML reports, pipeline integration, and usage documentation
* v0.6.31 pins the exact expected toolchain with CMake 4.4.2 and Qt 6.11.1 as hard requirements instead of version floors, and documents every dependency in the README
* v0.6.32 adds a GitHub Actions workflow that runs the local pipeline on every push and pull request with the same exactly pinned CMake, Qt and gcovr releases
* v0.6.33 adds the CI status badge and badges for the pinned license, Qt, CMake and C++ standard to the top of the README
* v0.6.34 documents the working agreements in AGENTS.md, covering the quality gate, atomic conventional commits, the per-commit version bump, the exact dependency pins, and the code, test and localization conventions
* v0.6.35 installs the qtdeclarative archive in CI, because the lrelease binary used to build the translations links against libQt6Qml and cannot start without it
* v0.6.36 resolves the repository paths in the version-metadata test from the CMake source directory instead of a hardcoded home path, so the test also runs on CI and in any other checkout
* v0.6.37 installs the qtimageformats and qtsvg Qt components in CI, so the runner supports the same image formats as a desktop installation and the Extensions menu is populated identically
* v0.6.38 declares the two parameters that older Cppcheck releases flag as const, so the static analysis reports zero findings on the CI image as well as locally
* v0.6.39 skips the report auto-open step when no graphical session is present, so headless runs such as CI report it as skipped instead of warning and no longer probe for a text browser
* v0.6.40 raises the language standard from C++17 to C++23, the most recent published standard, and replaces the two remaining implicit `[=]` lambda captures of `this`, which are deprecated since C++20
* v0.6.41 moves the version history out of the README into this changelog, so the README stays about using and building the program
* v0.6.42 caches the supported-image-format lookup, drops the per-file logging from the directory rescan that runs after every move, scales the displayed photo smoothly, and adopts C++20 ranges, designated initializers and `[[nodiscard]]` getters
* v0.6.43 corrects the sample pipeline output in the README, which quoted two different coverage percentages from two different runs in the same block
* v0.6.44 adds install rules plus a desktop entry, an icon and AppStream metainfo whose version is generated from the CMake project version, so the application can be packaged for Linux desktops
* v0.6.45 adds a tag-triggered release workflow that verifies the tag against the project version, runs the full pipeline, builds an AppImage bundling Qt, smoke-tests that AppImage headless, and publishes it with notes taken from this changelog
* v0.6.46 drops Qt image format plugins whose system libraries the build image cannot satisfy before packaging, so the release no longer fails on the RHEL-built tiff plugin, and asserts that the webp reader and the desktop platform plugin survive into the AppImage
* v0.6.47 bundles the offscreen and Wayland platform plugins into the AppImage and runs the packaging smoke test isolated from the build machine's Qt, which had made the test pass using plugins the package does not actually contain
* v0.7.0 releases ready-to-run packages for both desktop platforms: the Linux AppImage now also carries the offscreen platform plugin, a Windows 11 x64 zip with the Qt and MSVC runtimes is built alongside it, and each package is started once from its own files before the release is published
