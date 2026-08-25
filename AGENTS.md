# AGENTS.md

Working agreements for this repository, for human contributors and for AI agents alike.
If anything here contradicts a general habit you brought with you, this file wins.

Cullendula is a Qt 6 widget application that helps pick the best shots out of a photo
session. It is a small, single-maintainer project with a deliberately strict quality
gate: the gate is the point, so do not work around it.

---

## 1. The golden rule

**Never commit without a green pipeline.**

```sh
./localPipeline.sh --noRun
```

`--noRun` skips the final interactive app launch and is the right invocation for any
non-interactive or automated context. Exit code `0` and every mandatory stage `PASS` is
the only acceptable state for a commit.

The pipeline runs, in order:

1. CMake configure and build
2. unit tests via CTest
3. a dedicated coverage build, plus a **90% line-coverage threshold** (currently 98.6%)
4. Doxygen, plus the requirement that `build/doxygen/warnings.txt` is **empty**
5. Cppcheck (XML plus HTML report)
6. clang-format over `src/` and `tests/`

Stages 1-6 are all mandatory for the exit code. `clang-format` reporting changed files is
a `WARN`, not a failure, but a commit should never contain unformatted code: run the
pipeline, let it format, and include the result.

If the pipeline cannot run for an environmental reason, say so plainly. Never describe a
run as green that did not happen.

---

## 2. Commits

### 2.1 One concern per commit

Commits are **atomic**. A dependency bump, a CI change, and a documentation change are
three commits, not one. If a change needs the word "and" to describe its purpose, it is
probably two commits.

Split a larger piece of work so that every individual commit leaves the repository in a
working state with a green pipeline. Do not leave a knowingly broken commit in history
with a follow-up fix on top: if an unpushed commit turns out to be wrong, unwind it
(`git reset HEAD~N`) and re-commit cleanly. Interactive rebase is not available in the
usual agent environment, so plan the sequence before you start committing.

### 2.2 Conventional commit messages

Subject line: `type(scope): summary in the imperative, lower case, no trailing period`.

Types in use here: `feat`, `fix`, `refactor`, `test`, `docs`, `build`, `ci`, `chore`.
Scopes are optional and short, for example `build(deps)`, `docs(readme)`, `test(coverage)`.

The body explains **why**, not what the diff already shows, and ends by stating the
verification result, for example the coverage percentage and that the pipeline passed.

**Never mention AI, Claude, an assistant, or any agent anywhere in a commit message,
in code comments, or in the documentation.** The commit history reads as the maintainer's
own work.

### 2.3 Every commit bumps the project version

There is no such thing as a commit without a version bump. Increment the patch component
(`0.6.33` becomes `0.6.34`) in the **same commit** as the change it describes.

A commit is therefore always a package of: the change, the version bump, and the history
entry describing it.

### 2.4 Never push

Commit locally and stop. The maintainer reviews the commits and pushes them. Do not
push, do not open pull requests, and do not create tags unless explicitly asked.

---

## 3. How to bump the version

The version lives in **four** places and a unit test enforces that they agree, so a
partial bump fails the pipeline:

| # | File | What to change |
| --- | --- | --- |
| 1 | `CMakeLists.txt` | `project(Cullendula VERSION x.y.z ...)` |
| 2 | `README.md` | the line `This is version x.y.z.` |
| 3 | `CHANGELOG.md` | append a new `* vx.y.z <what changed>` bullet at the end of the list |
| 4 | `tests/Test_CullendulaMainWindow.cpp` | the three `QVERIFY` lines in `slot_Test_VersionMetadata_IsDocumentedConsistently()` |

The test in place 4 reads the other three files and asserts they contain the new version,
including a prefix of the new changelog bullet. Keep the asserted prefix short enough that
minor rewording of the bullet does not break it.

`CULLENDULA_PROJECT_VERSION` is compiled from the CMake project version and shown in the
window title, and Doxygen takes its project number from the same value, so `CMakeLists.txt`
is the single source of truth. Never hardcode the version anywhere else.

---

## 4. Dependencies

### 4.1 Exact pins, not ranges

Dependencies are pinned to **exact releases**. Not floors, not ranges, not "or newer".
The two enforced pins live together at the top of `CMakeLists.txt`:

```cmake
set(CULLENDULA_EXPECTED_CMAKE_VERSION "4.4.2")
set(CULLENDULA_EXPECTED_QT_VERSION "6.11.1")
```

* CMake is checked with `VERSION_EQUAL` and a `FATAL_ERROR` on mismatch.
* Qt is found with `find_package(Qt6 ${CULLENDULA_EXPECTED_QT_VERSION} EXACT REQUIRED ...)`,
  so a different patch release fails to configure.

`gcovr` is pinned in CI (`gcovr==8.6`). GCC, Cppcheck, Doxygen and clang-format are
distribution packages whose exact release differs between a rolling desktop and the CI
image; they are documented in the README dependency table and reported by CI, but not
hard-pinned, because failing on them would make the project unbuildable without improving
the produced binary.

### 4.2 Changing a pin

Look the version up at the real upstream registry, never from memory:

* CMake: <https://api.github.com/repos/Kitware/CMake/releases/latest>
* Qt: <https://download.qt.io/official_releases/qt/>
* PyPI packages: `https://pypi.org/pypi/<name>/json` → `.info.version`
* GitHub Actions: the action repository's latest release

Confirm the candidate is a stable release and not a pre-release, then update **all** of:

1. the `CULLENDULA_EXPECTED_*_VERSION` value in `CMakeLists.txt`
2. the matching `env:` value in `.github/workflows/ci.yml`
3. the dependency table and the badges in `README.md`
4. section 4.1 of this file

Only pin a version you can actually build and verify against locally. Pinning a release
that is not installable here would mean shipping a repository whose pipeline cannot run.

---

## 5. Continuous integration

`.github/workflows/ci.yml` runs `./localPipeline.sh --noRun --verbose` on `ubuntu-24.04`
for pushes to `master`, pull requests against `master`, and manual dispatch.

CI deliberately calls the same script that is used locally, so the two cannot drift apart.
**When the gate changes, change `localPipeline.sh`, never the workflow's command list.**
The workflow's own job is only to install the pinned toolchain and to publish reports.

All action versions are pinned to exact releases, never to a floating major tag.

`.github/workflows/release.yml` publishes a release when a `v*` tag is pushed. It refuses
a tag that disagrees with the version in `CMakeLists.txt` or that has no `CHANGELOG.md`
entry, then builds a Linux AppImage and a Windows zip in parallel and starts each of
them once before publishing. A package that cannot start is not a release.

When testing a package, remove `QT_PLUGIN_PATH`, `QT_ROOT_DIR` and `LD_LIBRARY_PATH`
from the environment first, or on Windows run from inside the package folder. The Qt
install action exports those, and a package will happily use the build machine's Qt
instead of its own, which makes a broken package look fine.
Packaging metadata lives in `packaging/`; the AppStream version is generated from the
CMake project version, so never hardcode a version there.

CI must install the Qt **components** the application needs at run time, not only the
ones it needs to compile. The Extensions menu is built from
`QImageReader::supportedImageFormats()`, so image-format plugins change observable
behaviour and therefore test results: `qtimageformats` provides webp and tiff, `qtsvg`
provides svg, and `qtdeclarative` is required merely because `lrelease` links against
libQt6Qml. A runner with a thinner Qt than a desktop installation produces failures that
cannot be reproduced locally. To reproduce one deliberately, copy the Qt plugin directory,
delete the plugins in question, and point the test binary at the copy with a `qt.conf`
containing a `[Paths]` `Plugins=` entry.

---

## 6. Code style

* C++23, enforced by `CMAKE_CXX_STANDARD_REQUIRED`. It is the most recent published
  standard; C++26 is still a working draft and the CI compiler does not offer it.
* Capture `this` explicitly in lambdas (`[this]`), never through `[=]`: the implicit
  capture of `this` via `[=]` is deprecated since C++20 and warns on the CI compiler.
* Formatting is `clang-format` with the checked-in `.clang-format` (Google style, 4-space
  indent, 160 column limit). Never hand-format against it; let the pipeline format.
* Headers start with the four-line banner comment (description, author, repo) and use
  `#pragma once`.
* Qt includes carry their module prefix: `#include <QtCore/QString>`, not `<QString>`.
* East const: `QString const& path`.
* `//---...---` separator lines between function definitions in `.cpp` files.
* Public API carries Doxygen comments (`/*! @brief ... */`, `@param`, `@return`). Doxygen
  warnings are a hard gate, so an undocumented parameter breaks the build.
* Comments explain **why**. Do not add comments that restate the code; stale and
  low-signal comments have been deliberately removed from this codebase before.

---

## 7. Tests

* QtTest, in `tests/`, linked against `CullendulaLib`.
* Test slots are named `slot_Test_<Subject>_<Expectation>()`.
* Tests must run headless: the harness falls back to `QT_QPA_PLATFORM=offscreen`, so never
  require a real display or user interaction.
* Line coverage must stay at or above 90%. It currently sits at 98.6% — do not let a
  change erode it; add tests together with the code that needs them.
* Cppcheck findings are kept at zero. Fix the real issue where there is one. Where a
  pattern is deliberate, suppress it narrowly and inline with `// cppcheck-suppress <id>`
  plus a comment stating *why*, never by broadening the global suppression list.

---

## 8. Localization

Three translations are maintained: German, Croatian and Chinese (`translations/*.ts`).

* User-visible strings must be wrapped for translation, with a translator comment where
  the string is ambiguous.
* `.qm` generation is automatic during the build.
* Refreshing the `.ts` files is manual and deliberate:
  `cmake --build build --target update_translations`.

---

## 9. Housekeeping

* `build/`, `build-coverage/`, `reports/` and `.localPipeline/` are generated and
  git-ignored. Never commit them, and never commit generated documentation or reports.
* `llm_compat_output.md` is a generated dump and is git-ignored.
* When a change alters behaviour or the toolchain, update `README.md` in the same commit.
  Documentation drift counts as an incomplete change.
