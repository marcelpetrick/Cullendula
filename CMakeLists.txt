cmake_minimum_required(VERSION 3.28)

project(Cullendula
    VERSION 0.5.0
    LANGUAGES CXX
)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt auto-processing for  MOC/UIC/RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt 5.15
find_package(Qt5 5.15 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Test
)

# Build the application code
add_subdirectory(src)

# Enable testing
enable_testing()
add_subdirectory(tests)
