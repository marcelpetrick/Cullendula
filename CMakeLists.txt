cmake_minimum_required(VERSION 3.28)

# Silence Qt Creator MaintenanceTool provider message (must be set early)
set(QT_CREATOR_SKIP_MAINTENANCE_TOOL_PROVIDER ON)

project(Cullendula
    VERSION 0.5.5
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

# Find Qt 5.15
find_package(Qt5 5.15 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
)

if(CULLENDULA_BUILD_TESTS)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Test)
endif()

# Build the application code
add_subdirectory(src)

# Enable testing
if(CULLENDULA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
