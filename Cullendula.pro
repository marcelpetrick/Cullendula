#----------------------------------------------------------------------------------
# pro file for the application "Cullendula"
#
# description: Cullendula - small GUI-app to pick the best shots from a session
# author: mail@marcelpetrick.it
# repo: https://github.com/marcelpetrick/Cullendula
#----------------------------------------------------------------------------------

QT += core gui
# for the qtest
QT += widgets testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
# last line, the main, has to be removed in case of unit-testing because of the expanding QTEST_MAIN-macro

HEADERS += \
	CullendulaMainWindow.h \
	CullendulaFileSystemHandler.h \
	CullendulaUndoStack.h \
	Test_CullendulaUndoStack.h

FORMS += \
	CullendulaMainWindow.ui
