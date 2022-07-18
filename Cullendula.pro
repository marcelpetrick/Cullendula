#----------------------------------------------------------------------------------
# pro file for the application "Cullendula"
#
# description: Cullendula - small GUI-app to pick quickly the best shots from a session
#              The name is a wordplay on "to cull" and a local plant (Calendula).
#
# author: mail@marcelpetrick.it
# repo: https://github.com/marcelpetrick/Cullendula
#
# requires: Qt5
# tested with Qt 5.12.5, 5.15.1, 5.15.2 (last FOSS LTS) and 6.0 beta
#
#----------------------------------------------------------------------------------

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
    src \
    tests
