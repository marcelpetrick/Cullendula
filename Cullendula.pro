#----------------------------------------------------------------------------------
# pro file for the application "Cullendula"
#
# description: Cullendula - small GUI-app to pick quickly the best shots from a session
# author: mail@marcelpetrick.it
# repo: https://github.com/marcelpetrick/Cullendula
#
# requires: Qt5
# tested with Qt 5.12.5, 5.15.1 and 6.0 beta
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
    src/CullendulaMainWindow.cpp \
    src/CullendulaFileSystemHandler.cpp \
    src/CullendulaUndoStack.cpp \
    src/Test_CullendulaUndoStack.cpp \
    src/main.cpp

HEADERS += \
    src/CullendulaMainWindow.h \
    src/CullendulaFileSystemHandler.h \
    src/CullendulaUndoStack.h \
    src/Test_CullendulaUndoStack.h

FORMS += \
    src/CullendulaMainWindow.ui

RC_FILE += \
    media\cullendula.rc
