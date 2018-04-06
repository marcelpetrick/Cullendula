#----------------------------------------------------------------------------------
# pro file for the application "Cullendula"
#
# description: Cullendula - small GUI-app to pick the best shots from a session
# author: mail@marcelpetrick.it
# repo: https://github.com/marcelpetrick/Cullendula
#----------------------------------------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cullendula
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
	main.cpp \
	CullendulaMainWindow.cpp \
	CullendulaFileSystemHandler.cpp

HEADERS += \
	CullendulaMainWindow.h \
	CullendulaFileSystemHandler.h

FORMS += \
	CullendulaMainWindow.ui
