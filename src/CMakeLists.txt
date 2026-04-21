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

target_link_libraries(CullendulaLib
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# 🔴 NEW: expose src/ as public include dir for anything linking CullendulaLib
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
    ../media/cullendula.rc
)

target_link_libraries(Cullendula
    PRIVATE CullendulaLib
)

# Windows subsystem settings (optional)
if(WIN32)
    set_target_properties(Cullendula PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
