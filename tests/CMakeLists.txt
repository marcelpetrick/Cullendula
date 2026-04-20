#
# Test sources
#
set(TEST_SOURCES
    main.cpp
    Test_CullendulaUndoStack.cpp
    Test_CullendulaUndoStack.h
)

#
# Test executable
#
add_executable(CullendulaTests ${TEST_SOURCES})

#
# Link against QtTest + the library containing the app code
#
target_link_libraries(CullendulaTests
    Qt5::Core
    Qt5::Gui          # needed because UndoStack uses Qt GUI types
    Qt5::Test
    CullendulaLib
)

# 🔴 NEW: make sure tests can #include "CullendulaUndoStack.h"
target_include_directories(CullendulaTests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

#
# Register test with CTest
#
add_test(NAME CullendulaUndoStackTest COMMAND CullendulaTests)

add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS CullendulaTests
    USES_TERMINAL
)

if(CULLENDULA_ENABLE_COVERAGE)
    find_program(CULLENDULA_GCOV_EXECUTABLE gcov REQUIRED)
    find_program(CULLENDULA_GCOVR_EXECUTABLE gcovr)
    find_program(CULLENDULA_LCOV_EXECUTABLE lcov)
    find_program(CULLENDULA_GENHTML_EXECUTABLE genhtml)

    set(CULLENDULA_COVERAGE_OBJECTS
        "${CMAKE_BINARY_DIR}/src/CMakeFiles/CullendulaLib.dir/CullendulaUndoStack.cpp.gcno"
    )

    set(CULLENDULA_COVERAGE_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/CoverageConfig.cmake")
    file(GENERATE OUTPUT "${CULLENDULA_COVERAGE_CONFIG_FILE}" CONTENT
"set(COVERAGE_OBJECTS
    [==[${CULLENDULA_COVERAGE_OBJECTS}]==]
)
")

    add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMAND ${CMAKE_COMMAND}
            -DGCOV_EXECUTABLE=${CULLENDULA_GCOV_EXECUTABLE}
            -DCOVERAGE_OUTPUT_DIR=${CMAKE_BINARY_DIR}/coverage
            -DCOVERAGE_REPORT_FILE=${CMAKE_BINARY_DIR}/coverage/coverage.txt
            -DCOVERAGE_CONFIG_FILE=${CULLENDULA_COVERAGE_CONFIG_FILE}
            -P ${CMAKE_SOURCE_DIR}/cmake/GenerateCoverage.cmake
        DEPENDS CullendulaTests
        USES_TERMINAL
    )

    if(CULLENDULA_GCOVR_EXECUTABLE)
        add_custom_target(coverage-html
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage/html
            COMMAND ${CULLENDULA_GCOVR_EXECUTABLE}
                --root ${CMAKE_SOURCE_DIR}
                --filter ${CMAKE_SOURCE_DIR}/src
                --html-details ${CMAKE_BINARY_DIR}/coverage/html/index.html
                --gcov-executable ${CULLENDULA_GCOV_EXECUTABLE}
                ${CMAKE_BINARY_DIR}
            DEPENDS CullendulaTests
            USES_TERMINAL
        )
    elseif(CULLENDULA_LCOV_EXECUTABLE AND CULLENDULA_GENHTML_EXECUTABLE)
        add_custom_target(coverage-html
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
            COMMAND ${CULLENDULA_LCOV_EXECUTABLE}
                --capture
                --directory ${CMAKE_BINARY_DIR}
                --output-file ${CMAKE_BINARY_DIR}/coverage/coverage.info
            COMMAND ${CULLENDULA_LCOV_EXECUTABLE}
                --extract ${CMAKE_BINARY_DIR}/coverage/coverage.info
                ${CMAKE_SOURCE_DIR}/src/*
                --output-file ${CMAKE_BINARY_DIR}/coverage/coverage.filtered.info
            COMMAND ${CULLENDULA_GENHTML_EXECUTABLE}
                ${CMAKE_BINARY_DIR}/coverage/coverage.filtered.info
                --output-directory ${CMAKE_BINARY_DIR}/coverage/html
            DEPENDS CullendulaTests
            USES_TERMINAL
        )
    else()
        message(STATUS "coverage-html target disabled: install gcovr or lcov+genhtml to enable HTML coverage output")
    endif()
endif()
