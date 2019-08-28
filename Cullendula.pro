#----------------------------------------------------------------------------------
# pro file for the application "Cullendula"
#
# description: Cullendula - small GUI-app to pick quickly the best shots from a session
# author: mail@marcelpetrick.it
# repo: https://github.com/marcelpetrick/Cullendula
#
# requires: Qt5
#
#----------------------------------------------------------------------------------

QT += \
    core \
    gui \
    widgets \
    testlib

TARGET = Cullendula
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    CullendulaMainWindow.cpp \
    CullendulaFileSystemHandler.cpp \
    CullendulaUndoStack.cpp \
    Test_CullendulaUndoStack.cpp \
    main.cpp

HEADERS += \
    CullendulaMainWindow.h \
    CullendulaFileSystemHandler.h \
    CullendulaUndoStack.h \
    Test_CullendulaUndoStack.h

FORMS += \
    CullendulaMainWindow.ui

RC_FILE += \
    cullendula.rc
