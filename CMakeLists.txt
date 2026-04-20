cmake_minimum_required(VERSION 3.28)

# Silence Qt Creator MaintenanceTool provider message (must be set early)
set(QT_CREATOR_SKIP_MAINTENANCE_TOOL_PROVIDER ON)

project(Cullendula
    VERSION 0.6.8
    LANGUAGES CXX
)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt auto-processing for MOC/UIC/RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Allow configuring without tests if desired (useful for IDE/GUIs)
option(CULLENDULA_BUILD_TESTS "Build tests" ON)
option(CULLENDULA_ENABLE_COVERAGE "Enable gcov coverage instrumentation" OFF)

if(CULLENDULA_ENABLE_COVERAGE AND NOT CULLENDULA_BUILD_TESTS)
    message(FATAL_ERROR "CULLENDULA_ENABLE_COVERAGE requires CULLENDULA_BUILD_TESTS=ON")
endif()

if(CULLENDULA_ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-O0 -g --coverage)
        add_link_options(--coverage)
    else()
        message(FATAL_ERROR "Coverage is only supported with GCC or Clang compatible coverage flags")
    endif()
endif()

# Find Qt 6.10
find_package(Qt6 6.10 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
)

if(CULLENDULA_BUILD_TESTS)
    find_package(Qt6 6.10 REQUIRED COMPONENTS Test)
endif()

find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(CULLENDULA_DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/doxygen")
    set(CULLENDULA_DOXYGEN_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

    configure_file(
        "${CMAKE_SOURCE_DIR}/Doxyfile.in"
        "${CULLENDULA_DOXYGEN_CONFIG_FILE}"
        @ONLY
    )

    add_custom_target(doxygen
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CULLENDULA_DOXYGEN_OUTPUT_DIRECTORY}"
        COMMAND ${DOXYGEN_EXECUTABLE} "${CULLENDULA_DOXYGEN_CONFIG_FILE}"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Generating HTML API documentation with Doxygen"
        VERBATIM
    )
else()
    message(STATUS "Doxygen not found: the 'doxygen' target will be unavailable")
endif()

# Build the application code
add_subdirectory(src)

# Enable testing
if(CULLENDULA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
