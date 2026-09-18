#
# Library containing all core + UI logic (except main.cpp)
#
add_library(CullendulaLib
    CullendulaAppBootstrap.cpp
    CullendulaMainWindow.cpp
    CullendulaFileSystemHandler.cpp
    CullendulaUndoStack.cpp

    CullendulaAppBootstrap.h
    CullendulaMainWindow.h
    CullendulaFileSystemHandler.h
    CullendulaUndoStack.h

    CullendulaMainWindow.ui
)

# PUBLIC keeps the previous plain-signature behaviour, where everything linking this
# library also links Qt. The keyword form is required because qt_add_resources below
# uses it internally, and CMake refuses to mix the two signatures on one target.
target_link_libraries(CullendulaLib
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

# Expose src/ headers to targets linking CullendulaLib.
target_include_directories(CullendulaLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(CullendulaLib
    PUBLIC
        CULLENDULA_PROJECT_VERSION="${PROJECT_VERSION}"
)

#
# GUI Application — links to the library
#
add_executable(Cullendula
    main.cpp
)

# The .rc only carries the Windows executable icon and needs a resource compiler,
# so it is attached on Windows instead of being listed unconditionally.
if(WIN32)
    target_sources(Cullendula PRIVATE ../media/cullendula.rc)
endif()

# The window icon is the very same file that is installed into hicolor and that the
# AppImage is built with, so the packaged icon and the running window cannot drift.
qt_add_resources(CullendulaLib "cullendula_icons"
    PREFIX "/icons"
    BASE "${CMAKE_SOURCE_DIR}/media"
    FILES "${CMAKE_SOURCE_DIR}/media/it.marcelpetrick.Cullendula.png"
)

target_link_libraries(Cullendula
    PRIVATE CullendulaLib
)

# Windows subsystem settings (optional)
if(WIN32)
    set_target_properties(Cullendula PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
