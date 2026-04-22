#!/usr/bin/env bash

set -u
set -o pipefail

readonly SCRIPT_NAME="$(basename "$0")"
readonly PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
readonly DEFAULT_BUILD_DIR="${PROJECT_ROOT}/build"
readonly DEFAULT_REPORT_DIR="${PROJECT_ROOT}/reports/cppcheck"

BUILD_DIR="${DEFAULT_BUILD_DIR}"
REPORT_DIR="${DEFAULT_REPORT_DIR}"
VERBOSE=0

print_usage() {
    cat <<EOF
Usage: ${SCRIPT_NAME} [--build-dir PATH] [--report-dir PATH] [--verbose] [--help]

Run Cppcheck against the project's compilation database and generate reports.

Outputs:
  - XML report:  <report-dir>/cppcheck.xml
  - HTML report: <report-dir>/html/index.html (when cppcheck-htmlreport is available)
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

run_command() {
    if [[ "${VERBOSE}" -eq 1 ]]; then
        "$@"
    else
        "$@" >/dev/null
    fi
}

parse_arguments() {
    while [[ "$#" -gt 0 ]]; do
        case "$1" in
            --build-dir)
                shift
                if [[ "$#" -eq 0 ]]; then
                    error "Missing value for --build-dir."
                    exit 2
                fi
                BUILD_DIR="$1"
                ;;
            --report-dir)
                shift
                if [[ "$#" -eq 0 ]]; then
                    error "Missing value for --report-dir."
                    exit 2
                fi
                REPORT_DIR="$1"
                ;;
            --verbose)
                VERBOSE=1
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

require_command() {
    local command_name="$1"
    if ! command -v "${command_name}" >/dev/null 2>&1; then
        error "Required command '${command_name}' was not found in PATH."
        exit 1
    fi
}

extract_finding_count() {
    local xml_file="$1"
    python3 - "${xml_file}" <<'PY'
import sys
import xml.etree.ElementTree as ET

xml_path = sys.argv[1]
root = ET.parse(xml_path).getroot()
errors = root.find("errors")
print(0 if errors is None else len(errors.findall("error")))
PY
}

main() {
    parse_arguments "$@"

    require_command cppcheck
    require_command python3

    local compile_commands_file="${BUILD_DIR}/compile_commands.json"
    local xml_report="${REPORT_DIR}/cppcheck.xml"
    local html_report_dir="${REPORT_DIR}/html"
    local cppcheck_build_dir="${REPORT_DIR}/build"

    if [[ ! -f "${compile_commands_file}" ]]; then
        error "Compilation database not found: ${compile_commands_file}"
        error "Configure the project first so CMake generates compile_commands.json."
        exit 1
    fi

    mkdir -p "${REPORT_DIR}" "${cppcheck_build_dir}" || {
        error "Could not create report directory: ${REPORT_DIR}"
        exit 1
    }

    rm -f "${xml_report}"

    log "Running Cppcheck using ${compile_commands_file}"
    if ! run_command cppcheck \
        --project="${compile_commands_file}" \
        --enable=warning,style,performance,portability \
        --xml \
        --xml-version=2 \
        --output-file="${xml_report}" \
        --inline-suppr \
        --library=qt \
        --cppcheck-build-dir="${cppcheck_build_dir}" \
        --file-filter="${PROJECT_ROOT}/src/*" \
        --file-filter="${PROJECT_ROOT}/tests/*" \
        -i "${PROJECT_ROOT}/build" \
        -i "${PROJECT_ROOT}/build-coverage" \
        -i "${PROJECT_ROOT}/.localPipeline" \
        -i "${PROJECT_ROOT}/reports"; then
        error "Cppcheck execution failed."
        exit 1
    fi

    if [[ ! -s "${xml_report}" ]]; then
        error "Cppcheck did not produce a non-empty XML report: ${xml_report}"
        exit 1
    fi

    local finding_count
    finding_count="$(extract_finding_count "${xml_report}")" || {
        error "Could not parse the Cppcheck XML report."
        exit 1
    }

    log "Cppcheck XML report: ${xml_report}"
    log "Cppcheck findings: ${finding_count}"

    if command -v cppcheck-htmlreport >/dev/null 2>&1; then
        mkdir -p "${html_report_dir}" || {
            error "Could not create HTML report directory: ${html_report_dir}"
            exit 1
        }

        log "Generating Cppcheck HTML report."
        if ! run_command cppcheck-htmlreport \
            --file="${xml_report}" \
            --report-dir="${html_report_dir}" \
            --source-dir="${PROJECT_ROOT}"; then
            error "Cppcheck HTML report generation failed."
            exit 1
        fi

        if [[ ! -f "${html_report_dir}/index.html" ]]; then
            error "Cppcheck HTML report entry point was not created: ${html_report_dir}/index.html"
            exit 1
        fi

        log "Cppcheck HTML report: ${html_report_dir}/index.html"
    else
        warn "cppcheck-htmlreport is not available; skipping HTML report generation."
    fi
}

main "$@"
