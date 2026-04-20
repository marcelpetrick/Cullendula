if(NOT DEFINED GCOV_EXECUTABLE OR GCOV_EXECUTABLE STREQUAL "")
    message(FATAL_ERROR "GCOV_EXECUTABLE must be set")
endif()

if(NOT DEFINED COVERAGE_OUTPUT_DIR OR COVERAGE_OUTPUT_DIR STREQUAL "")
    message(FATAL_ERROR "COVERAGE_OUTPUT_DIR must be set")
endif()

if(NOT DEFINED COVERAGE_REPORT_FILE OR COVERAGE_REPORT_FILE STREQUAL "")
    message(FATAL_ERROR "COVERAGE_REPORT_FILE must be set")
endif()

if(DEFINED COVERAGE_CONFIG_FILE AND NOT COVERAGE_CONFIG_FILE STREQUAL "")
    include("${COVERAGE_CONFIG_FILE}")
endif()

if(NOT DEFINED COVERAGE_OBJECTS OR COVERAGE_OBJECTS STREQUAL "")
    message(FATAL_ERROR "COVERAGE_OBJECTS must be set")
endif()

set(coverage_objects ${COVERAGE_OBJECTS})

file(MAKE_DIRECTORY "${COVERAGE_OUTPUT_DIR}")
file(WRITE "${COVERAGE_REPORT_FILE}" "Cullendula coverage report\n")
file(APPEND "${COVERAGE_REPORT_FILE}" "Generated with gcov\n\n")

execute_process(
    COMMAND "${CMAKE_COMMAND}" -E remove_directory "${COVERAGE_OUTPUT_DIR}/gcov"
)
file(MAKE_DIRECTORY "${COVERAGE_OUTPUT_DIR}/gcov")

list(LENGTH coverage_objects coverage_object_count)
math(EXPR last_index "${coverage_object_count} - 1")
foreach(index RANGE ${last_index})
    list(GET coverage_objects ${index} coverage_object)

    execute_process(
        COMMAND "${GCOV_EXECUTABLE}" -b -c "${coverage_object}"
        WORKING_DIRECTORY "${COVERAGE_OUTPUT_DIR}/gcov"
        RESULT_VARIABLE coverage_result
        OUTPUT_VARIABLE coverage_stdout
        ERROR_VARIABLE coverage_stderr
    )

    file(APPEND "${COVERAGE_REPORT_FILE}" "== ${coverage_object} ==\n")
    if(NOT coverage_stdout STREQUAL "")
        file(APPEND "${COVERAGE_REPORT_FILE}" "${coverage_stdout}")
    endif()

    if(NOT coverage_stderr STREQUAL "")
        file(APPEND "${COVERAGE_REPORT_FILE}" "${coverage_stderr}")
    endif()

    file(APPEND "${COVERAGE_REPORT_FILE}" "\n")

    if(NOT coverage_result EQUAL 0)
        message(FATAL_ERROR "gcov failed for ${coverage_object}")
    endif()
endforeach()

message(STATUS "Coverage report written to ${COVERAGE_REPORT_FILE}")
