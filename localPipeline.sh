#!/usr/bin/env bash

# -----------------------------------------------------------------------------
# localPipeline.sh
#
# Purpose:
#   Run a local, developer-oriented verification pipeline for the Cullendula
#   project and summarize the outcome in one place.
#
# What it does:
#   1. Configure the project with CMake
#   2. Build the configured targets in parallel
#   3. Run the unit test suite with CTest, using parallel workers when possible
#   4. Build a dedicated coverage configuration and generate an HTML report
#   5. Fail if total line coverage is below 90%
#   6. Generate Doxygen documentation
#   7. Verify that the Doxygen warnings file exists and is empty
#   8. Print the generated Doxygen index.html path and try to open it
#   9. Run clang-format on the C++ sources in src/ and tests/
#   10. Detect whether clang-format changed any files
#   11. Print a final stage-by-stage summary and exit with a useful status code
#
# Invocation:
#   ./localPipeline.sh
#   ./localPipeline.sh --verbose
#   ./localPipeline.sh --build-dir /absolute/or/relative/build-dir
#   ./localPipeline.sh --coverage-build-dir /absolute/or/relative/coverage-build-dir
#   CULLENDULA_PIPELINE_JOBS=8 ./localPipeline.sh
#   ./localPipeline.sh --help
#
# Exit codes:
#   0  All mandatory stages succeeded:
#      configure/build, tests, coverage generation, coverage threshold,
#      Doxygen generation, zero Doxygen warnings, and clang-format execution.
#   1  One or more mandatory stages failed.
#   2  Invalid command line arguments.
# -----------------------------------------------------------------------------

set -u
set -o pipefail

readonly SCRIPT_NAME="$(basename "$0")"
readonly PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly DEFAULT_BUILD_DIR="${PROJECT_ROOT}/build"
readonly DEFAULT_COVERAGE_BUILD_DIR="${PROJECT_ROOT}/build-coverage"
readonly STAGE_LOG_DIR="${PROJECT_ROOT}/.localPipeline"

VERBOSE=0
BUILD_DIR="${DEFAULT_BUILD_DIR}"
COVERAGE_BUILD_DIR="${DEFAULT_COVERAGE_BUILD_DIR}"
CTEST_COMMAND=""
PARALLEL_JOBS=1
GCOV_COMMAND=""
GCOVR_COMMAND=""
readonly COVERAGE_MIN_LINE_PERCENT="90.0"
PYTHON3_COMMAND=""

declare -a SUMMARY_LINES=()
declare -a FORMAT_TARGETS=()
declare -a FORMAT_CHANGED_FILES=()

BUILD_OK=0
TESTS_OK=0
COVERAGE_OK=0
COVERAGE_THRESHOLD_OK=0
OPEN_COVERAGE_OK=0
DOXYGEN_OK=0
DOXYGEN_WARNINGS_OK=0
OPEN_DOCS_OK=0
FORMAT_OK=0
FORMAT_CHANGED=0
COVERAGE_LINE_PERCENT=""

print_usage() {
    cat <<EOF
Usage: ${SCRIPT_NAME} [--verbose] [--build-dir PATH] [--coverage-build-dir PATH] [--help]

Local project pipeline:
  1. Configure and build the project
  2. Run unit tests
  3. Build a dedicated coverage tree, generate coverage, and enforce a 90% line coverage threshold
  4. Generate Doxygen documentation
  5. Check that the Doxygen warning log is empty
  6. Open the generated HTML reports when possible
  7. Run clang-format on project C++ sources
  8. Report whether formatting changed any files
EOF
}

log() {
    printf '[INFO] %s\n' "$*"
}

warn() {
    printf '[WARN] %s\n' "$*" >&2
}

error() {
    printf '[ERROR] %s\n' "$*" >&2
}

verbose_log() {
    if [[ "${VERBOSE}" -eq 1 ]]; then
        printf '[VERBOSE] %s\n' "$*"
    fi
}

add_summary() {
    SUMMARY_LINES+=("$1")
}

mark_result() {
    local label="$1"
    local status="$2"
    local details="$3"
    add_summary "$(printf '%-18s : %-4s %s' "${label}" "${status}" "${details}")"
}

run_command() {
    local description="$1"
    shift
    local log_file=""

    verbose_log "${description}"
    verbose_log "Command: $*"

    if [[ "${VERBOSE}" -eq 1 ]]; then
        "$@"
    else
        mkdir -p "${STAGE_LOG_DIR}" || {
            error "Could not create stage log directory: ${STAGE_LOG_DIR}"
            return 1
        }

        log_file="$(mktemp "${STAGE_LOG_DIR}/$(printf '%s' "${description}" | tr ' /' '__').XXXXXX.log")" || {
            error "Could not create a temporary log file for '${description}'."
            return 1
        }

        if "$@" >"${log_file}" 2>&1; then
            rm -f "${log_file}"
            return 0
        fi

        error "${description} failed. Captured output:"
        sed 's/^/  | /' "${log_file}" >&2
        return 1
    fi
}

require_command() {
    local command_name="$1"
    if ! command -v "${command_name}" >/dev/null 2>&1; then
        error "Required command '${command_name}' was not found in PATH."
        return 1
    fi
    return 0
}

resolve_ctest_command() {
    local cmake_path=""
    local cmake_dir=""
    local sibling_ctest=""

    cmake_path="$(command -v cmake 2>/dev/null)" || return 1
    cmake_dir="$(dirname "${cmake_path}")"
    sibling_ctest="${cmake_dir}/ctest"

    if [[ -x "${sibling_ctest}" ]]; then
        printf '%s\n' "${sibling_ctest}"
        return 0
    fi

    if command -v ctest >/dev/null 2>&1; then
        command -v ctest
        return 0
    fi

    return 1
}

extract_summary_json_number() {
    local json_file="$1"
    local key="$2"

    "${PYTHON3_COMMAND}" - "${json_file}" "${key}" <<'PY'
import json
import sys

json_path = sys.argv[1]
key = sys.argv[2]

with open(json_path, "r", encoding="utf-8") as handle:
    payload = json.load(handle)

value = payload.get(key)
if value is None:
    raise SystemExit(1)

print(value)
PY
}

detect_parallel_jobs() {
    local configured_jobs="${CULLENDULA_PIPELINE_JOBS:-}"
    local detected_jobs=""

    if [[ -n "${configured_jobs}" ]]; then
        if [[ "${configured_jobs}" =~ ^[1-9][0-9]*$ ]]; then
            printf '%s\n' "${configured_jobs}"
            return 0
        fi

        error "CULLENDULA_PIPELINE_JOBS must be a positive integer, got '${configured_jobs}'."
        return 1
    fi

    if command -v nproc >/dev/null 2>&1; then
        detected_jobs="$(nproc)"
    elif command -v getconf >/dev/null 2>&1; then
        detected_jobs="$(getconf _NPROCESSORS_ONLN 2>/dev/null || true)"
    elif command -v sysctl >/dev/null 2>&1; then
        detected_jobs="$(sysctl -n hw.ncpu 2>/dev/null || true)"
    fi

    if [[ "${detected_jobs}" =~ ^[1-9][0-9]*$ ]]; then
        printf '%s\n' "${detected_jobs}"
        return 0
    fi

    printf '%s\n' "1"
    return 0
}

detect_open_command() {
    if command -v xdg-open >/dev/null 2>&1; then
        printf '%s\n' "xdg-open"
        return 0
    fi

    if command -v open >/dev/null 2>&1; then
        printf '%s\n' "open"
        return 0
    fi

    return 1
}

collect_format_targets() {
    mapfile -t FORMAT_TARGETS < <(
        cd "${PROJECT_ROOT}" &&
            find src tests -type f \( -name '*.h' -o -name '*.cpp' \) | LC_ALL=C sort
    )

    if [[ "${#FORMAT_TARGETS[@]}" -eq 0 ]]; then
        warn "No C++ source files were found below src/ or tests/."
        return 1
    fi

    verbose_log "Formatting targets (${#FORMAT_TARGETS[@]} files):"
    if [[ "${VERBOSE}" -eq 1 ]]; then
        local target
        for target in "${FORMAT_TARGETS[@]}"; do
            printf '  - %s\n' "${target}"
        done
    fi

    return 0
}

checksum_files() {
    (
        cd "${PROJECT_ROOT}" || exit 1
        sha256sum "$@"
    )
}

find_changed_files_from_checksums() {
    local before_file="$1"
    local after_file="$2"

    awk '
        NR == FNR {
            before[$2] = $1
            next
        }
        {
            if (!($2 in before) || before[$2] != $1) {
                print $2
            }
        }
    ' "${before_file}" "${after_file}"
}

configure_and_build() {
    log "Configuring project in '${BUILD_DIR}'."
    mkdir -p "${BUILD_DIR}" || return 1

    if ! run_command "Running CMake configure step" cmake -S "${PROJECT_ROOT}" -B "${BUILD_DIR}" -DCULLENDULA_BUILD_TESTS=ON; then
        error "CMake configure step failed."
        return 1
    fi

    log "Building project with ${PARALLEL_JOBS} parallel job(s)."
    if ! run_command "Running project build" cmake --build "${BUILD_DIR}" --parallel "${PARALLEL_JOBS}"; then
        error "Project build failed."
        return 1
    fi

    return 0
}

run_tests() {
    log "Running unit tests via CTest with ${PARALLEL_JOBS} parallel job(s)."
    if ! run_command "Executing CTest" "${CTEST_COMMAND}" --test-dir "${BUILD_DIR}" --output-on-failure --parallel "${PARALLEL_JOBS}"; then
        error "Unit tests failed."
        return 1
    fi

    return 0
}

configure_and_build_coverage() {
    log "Configuring dedicated coverage build in '${COVERAGE_BUILD_DIR}'."
    mkdir -p "${COVERAGE_BUILD_DIR}" || return 1

    if ! run_command "Running CMake coverage configure step" cmake -S "${PROJECT_ROOT}" -B "${COVERAGE_BUILD_DIR}" -DCULLENDULA_BUILD_TESTS=ON -DCULLENDULA_ENABLE_COVERAGE=ON; then
        error "CMake coverage configure step failed."
        return 1
    fi

    log "Building coverage configuration with ${PARALLEL_JOBS} parallel job(s)."
    if ! run_command "Running coverage build" cmake --build "${COVERAGE_BUILD_DIR}" --parallel "${PARALLEL_JOBS}"; then
        error "Coverage build failed."
        return 1
    fi

    return 0
}

generate_coverage() {
    local coverage_output_dir="${COVERAGE_BUILD_DIR}/coverage"
    local coverage_html_dir="${coverage_output_dir}/html"
    local coverage_index_file="${coverage_html_dir}/index.html"
    local coverage_summary_file="${coverage_output_dir}/summary.json"
    local open_command=""

    log "Generating coverage report."
    if ! run_command "Building the coverage-html target" cmake --build "${COVERAGE_BUILD_DIR}" --target coverage-html --parallel "${PARALLEL_JOBS}"; then
        error "Coverage HTML generation failed."
        return 1
    fi

    if ! run_command "Generating coverage JSON summary" "${GCOVR_COMMAND}" \
        --root "${PROJECT_ROOT}" \
        --filter "${PROJECT_ROOT}/src" \
        --gcov-executable "${GCOV_COMMAND}" \
        --json-summary "${coverage_summary_file}" \
        --json-summary-pretty \
        "${COVERAGE_BUILD_DIR}"; then
        error "Coverage summary generation failed."
        return 1
    fi

    if [[ ! -f "${coverage_index_file}" ]]; then
        error "Expected coverage HTML entry point was not created: ${coverage_index_file}"
        return 1
    fi

    if [[ ! -f "${coverage_summary_file}" ]]; then
        error "Expected coverage summary file was not created: ${coverage_summary_file}"
        return 1
    fi

    COVERAGE_LINE_PERCENT="$(extract_summary_json_number "${coverage_summary_file}" "line_percent")"
    if [[ -z "${COVERAGE_LINE_PERCENT}" ]]; then
        error "Could not parse line coverage percentage from ${coverage_summary_file}."
        return 1
    fi

    log "Coverage HTML entry point: ${coverage_index_file}"
    log "Total line coverage: ${COVERAGE_LINE_PERCENT}%"

    if awk "BEGIN { exit !(${COVERAGE_LINE_PERCENT} >= ${COVERAGE_MIN_LINE_PERCENT}) }"; then
        COVERAGE_THRESHOLD_OK=1
    else
        warn "Coverage is below the required threshold of ${COVERAGE_MIN_LINE_PERCENT}%."
    fi

    if open_command="$(detect_open_command)"; then
        log "Opening coverage report with '${open_command}'."
        if run_command "Opening coverage index.html" "${open_command}" "${coverage_index_file}"; then
            OPEN_COVERAGE_OK=1
        else
            warn "Could not open the generated coverage report automatically."
        fi
    else
        warn "No supported open command was found. Coverage report will not be opened automatically."
    fi

    return 0
}

generate_doxygen() {
    local warnings_file="${BUILD_DIR}/doxygen/warnings.txt"
    local index_file="${BUILD_DIR}/doxygen/html/index.html"
    local open_command=""

    log "Generating Doxygen documentation."
    if ! run_command "Building the doxygen target" cmake --build "${BUILD_DIR}" --target doxygen --parallel "${PARALLEL_JOBS}"; then
        error "Doxygen generation failed."
        return 1
    fi

    if [[ ! -f "${warnings_file}" ]]; then
        error "Expected Doxygen warnings file was not created: ${warnings_file}"
        return 1
    fi

    if [[ ! -s "${warnings_file}" ]]; then
        log "Doxygen warnings file is empty."
        DOXYGEN_WARNINGS_OK=1
    else
        warn "Doxygen warnings were reported. See: ${warnings_file}"
    fi

    if [[ ! -f "${index_file}" ]]; then
        error "Expected Doxygen index file was not created: ${index_file}"
        return 1
    fi

    log "Doxygen HTML entry point: ${index_file}"

    if open_command="$(detect_open_command)"; then
        log "Opening generated documentation with '${open_command}'."
        if run_command "Opening Doxygen index.html" "${open_command}" "${index_file}"; then
            OPEN_DOCS_OK=1
        else
            warn "Could not open the generated documentation automatically."
        fi
    else
        warn "No supported open command was found. Documentation will not be opened automatically."
    fi

    return 0
}

run_clang_format() {
    local before_checksums_file
    local after_checksums_file

    log "Running clang-format on project C++ sources."

    mkdir -p "${STAGE_LOG_DIR}" || {
        error "Could not create stage log directory: ${STAGE_LOG_DIR}"
        return 1
    }

    before_checksums_file="$(mktemp "${STAGE_LOG_DIR}/format.before.XXXXXX.txt")" || {
        error "Could not create the pre-format checksum file."
        return 1
    }
    after_checksums_file="$(mktemp "${STAGE_LOG_DIR}/format.after.XXXXXX.txt")" || {
        error "Could not create the post-format checksum file."
        rm -f "${before_checksums_file}"
        return 1
    }

    checksum_files "${FORMAT_TARGETS[@]}" >"${before_checksums_file}" || {
        error "Failed to capture file checksums before formatting."
        rm -f "${before_checksums_file}" "${after_checksums_file}"
        return 1
    }

    (
        cd "${PROJECT_ROOT}" || exit 1
        clang-format -i "${FORMAT_TARGETS[@]}"
    ) || {
        error "clang-format failed."
        rm -f "${before_checksums_file}" "${after_checksums_file}"
        return 1
    }

    checksum_files "${FORMAT_TARGETS[@]}" >"${after_checksums_file}" || {
        error "Failed to capture file checksums after formatting."
        rm -f "${before_checksums_file}" "${after_checksums_file}"
        return 1
    }

    mapfile -t FORMAT_CHANGED_FILES < <(find_changed_files_from_checksums "${before_checksums_file}" "${after_checksums_file}")
    rm -f "${before_checksums_file}" "${after_checksums_file}"

    if [[ "${#FORMAT_CHANGED_FILES[@]}" -gt 0 ]]; then
        FORMAT_CHANGED=1
        warn "clang-format changed one or more files."
        log "Changed files after formatting:"
        printf '%s\n' "${FORMAT_CHANGED_FILES[@]}" | sed 's/^/  - /'
    else
        log "clang-format left all tracked source files unchanged."
    fi

    return 0
}

print_summary() {
    printf '\n========== Local Pipeline Summary ==========\n'
    local line
    for line in "${SUMMARY_LINES[@]}"; do
        printf '%s\n' "${line}"
    done
    printf '============================================\n'
}

parse_arguments() {
    while [[ "$#" -gt 0 ]]; do
        case "$1" in
            --verbose)
                VERBOSE=1
                ;;
            --build-dir)
                shift
                if [[ "$#" -eq 0 ]]; then
                    error "Missing value for --build-dir."
                    exit 2
                fi
                if [[ "$1" = -* ]]; then
                    error "Invalid build directory value: '$1'."
                    exit 2
                fi
                BUILD_DIR="$1"
                ;;
            --coverage-build-dir)
                shift
                if [[ "$#" -eq 0 ]]; then
                    error "Missing value for --coverage-build-dir."
                    exit 2
                fi
                if [[ "$1" = -* ]]; then
                    error "Invalid coverage build directory value: '$1'."
                    exit 2
                fi
                COVERAGE_BUILD_DIR="$1"
                ;;
            --help|-h)
                print_usage
                exit 0
                ;;
            *)
                error "Unknown argument: $1"
                print_usage
                exit 2
                ;;
        esac
        shift
    done
}

main() {
    parse_arguments "$@"

    log "Project root: ${PROJECT_ROOT}"
    log "Build directory: ${BUILD_DIR}"
    log "Coverage build directory: ${COVERAGE_BUILD_DIR}"
    if [[ "${VERBOSE}" -eq 1 ]]; then
        log "Verbose logging is enabled."
    fi

    local missing_prerequisites=0

    require_command cmake || missing_prerequisites=1
    require_command clang-format || missing_prerequisites=1
    require_command sha256sum || missing_prerequisites=1
    require_command gcov || missing_prerequisites=1
    require_command gcovr || missing_prerequisites=1
    require_command python3 || missing_prerequisites=1

    if ! CTEST_COMMAND="$(resolve_ctest_command)"; then
        error "Could not resolve a usable CTest executable."
        missing_prerequisites=1
    else
        verbose_log "Using CTest executable: ${CTEST_COMMAND}"
    fi

    GCOV_COMMAND="$(command -v gcov 2>/dev/null || true)"
    GCOVR_COMMAND="$(command -v gcovr 2>/dev/null || true)"
    PYTHON3_COMMAND="$(command -v python3 2>/dev/null || true)"

    if ! PARALLEL_JOBS="$(detect_parallel_jobs)"; then
        missing_prerequisites=1
    else
        log "Parallel jobs: ${PARALLEL_JOBS}"
    fi

    if ! collect_format_targets; then
        missing_prerequisites=1
    fi

    if [[ "${missing_prerequisites}" -ne 0 ]]; then
        add_summary "Prerequisite check failed. See errors above."
        print_summary
        exit 1
    fi

    if configure_and_build; then
        BUILD_OK=1
        mark_result "Configure+Build" "PASS" "Project configured and built in ${BUILD_DIR}"
    else
        mark_result "Configure+Build" "FAIL" "Build step did not complete successfully"
    fi

    if [[ "${BUILD_OK}" -eq 1 ]]; then
        if run_tests; then
            TESTS_OK=1
            mark_result "Unit Tests" "PASS" "CTest completed without failures"
        else
            mark_result "Unit Tests" "FAIL" "CTest reported at least one failing test"
        fi
    else
        mark_result "Unit Tests" "SKIP" "Skipped because build failed"
    fi

    if [[ "${TESTS_OK}" -eq 1 ]]; then
        if configure_and_build_coverage && generate_coverage; then
            COVERAGE_OK=1
            mark_result "Coverage" "PASS" "Coverage HTML generated successfully in ${COVERAGE_BUILD_DIR}"
        else
            mark_result "Coverage" "FAIL" "Coverage generation failed in ${COVERAGE_BUILD_DIR}"
        fi
    else
        mark_result "Coverage" "SKIP" "Skipped because tests did not complete successfully"
    fi

    if [[ "${COVERAGE_OK}" -eq 1 ]]; then
        if [[ "${COVERAGE_THRESHOLD_OK}" -eq 1 ]]; then
            mark_result "Coverage Gate" "PASS" "Line coverage is ${COVERAGE_LINE_PERCENT}% (threshold ${COVERAGE_MIN_LINE_PERCENT}%)"
        else
            mark_result "Coverage Gate" "FAIL" "Line coverage is ${COVERAGE_LINE_PERCENT}% (threshold ${COVERAGE_MIN_LINE_PERCENT}%)"
        fi

        if [[ "${OPEN_COVERAGE_OK}" -eq 1 ]]; then
            mark_result "Open Coverage" "PASS" "Coverage index.html was handed to the desktop opener"
        else
            mark_result "Open Coverage" "WARN" "Coverage index.html path was printed but auto-open was unavailable or failed"
        fi
    else
        mark_result "Coverage Gate" "SKIP" "Skipped because coverage generation failed"
        mark_result "Open Coverage" "SKIP" "Skipped because coverage generation failed"
    fi

    if [[ "${BUILD_OK}" -eq 1 ]]; then
        if generate_doxygen; then
            DOXYGEN_OK=1
            mark_result "Doxygen" "PASS" "Documentation generated successfully"
        else
            mark_result "Doxygen" "FAIL" "Documentation generation failed"
        fi
    else
        mark_result "Doxygen" "SKIP" "Skipped because build failed"
    fi

    if [[ "${DOXYGEN_OK}" -eq 1 ]]; then
        if [[ "${DOXYGEN_WARNINGS_OK}" -eq 1 ]]; then
            mark_result "Doxygen Warnings" "PASS" "warnings.txt is empty"
        else
            mark_result "Doxygen Warnings" "FAIL" "warnings.txt contains entries"
        fi

        if [[ "${OPEN_DOCS_OK}" -eq 1 ]]; then
            mark_result "Open Docs" "PASS" "index.html was handed to the desktop opener"
        else
            mark_result "Open Docs" "WARN" "index.html path was printed but auto-open was unavailable or failed"
        fi
    else
        mark_result "Doxygen Warnings" "SKIP" "Skipped because Doxygen generation failed"
        mark_result "Open Docs" "SKIP" "Skipped because Doxygen generation failed"
    fi

    if run_clang_format; then
        FORMAT_OK=1
        if [[ "${FORMAT_CHANGED}" -eq 1 ]]; then
            mark_result "clang-format" "WARN" "Formatting completed and changed files"
        else
            mark_result "clang-format" "PASS" "Formatting completed without changing files"
        fi
    else
        mark_result "clang-format" "FAIL" "Formatting step failed"
    fi

    print_summary

    if [[ "${BUILD_OK}" -eq 1 && "${TESTS_OK}" -eq 1 && "${COVERAGE_OK}" -eq 1 && "${COVERAGE_THRESHOLD_OK}" -eq 1 && "${DOXYGEN_OK}" -eq 1 && "${DOXYGEN_WARNINGS_OK}" -eq 1 && "${FORMAT_OK}" -eq 1 ]]; then
        exit 0
    fi

    exit 1
}

main "$@"
