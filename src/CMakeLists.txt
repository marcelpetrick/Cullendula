#
# Library containing all core + UI logic (except main.cpp)
#
add_library(CullendulaLib
    CullendulaMainWindow.cpp
    CullendulaFileSystemHandler.cpp
    CullendulaUndoStack.cpp

    CullendulaMainWindow.h
    CullendulaFileSystemHandler.h
    CullendulaUndoStack.h

    CullendulaMainWindow.ui
    resources.qrc
)

target_link_libraries(CullendulaLib
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

# 🔴 NEW: expose src/ as public include dir for anything linking CullendulaLib
target_include_directories(CullendulaLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

#
# GUI Application — links to the library
#
add_executable(Cullendula
    main.cpp
)

if(WIN32)
    target_sources(Cullendula PRIVATE ../media/cullendula.rc)
endif()

if(APPLE AND EXISTS "${CMAKE_SOURCE_DIR}/media/cullendula_icon.icns")
    set_target_properties(Cullendula PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_ICON_FILE cullendula_icon.icns
    )

    target_sources(Cullendula PRIVATE ../media/cullendula_icon.icns)
    set_source_files_properties(../media/cullendula_icon.icns PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )
endif()

target_link_libraries(Cullendula
    PRIVATE CullendulaLib
)

# Windows subsystem settings (optional)
if(WIN32)
    set_target_properties(Cullendula PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
