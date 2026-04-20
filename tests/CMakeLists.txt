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
