TEMPLATE = app
TARGET = evilalarm
QT += maemo5 dbus
CONFIG += silent release

OBJECTS_DIR = $${PWD}/tmp
MOC_DIR = $${PWD}/tmp
VPATH = $${PWD}/src
INCLUDEPATH += $${PWD}/src

# Input
HEADERS += accelerometer.h preferences.h alarm.h daemon.h mainwindow.h
SOURCES += main.cpp accelerometer.cpp preferences.cpp alarm.cpp daemon.cpp mainwindow.cpp
