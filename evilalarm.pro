TEMPLATE = app
TARGET = evilalarm
QT += maemo5 dbus
CONFIG += silent release

OBJECTS_DIR = $${PWD}/tmp
MOC_DIR = $${PWD}/tmp
VPATH = $${PWD}/src
INCLUDEPATH += $${PWD}/src

HEADERS += accelerometer.h settings.h alarm.h daemon.h mainwindow.h\
	alarm_movement.h
SOURCES += main.cpp accelerometer.cpp settings.cpp alarm.cpp daemon.cpp mainwindow.cpp\
	alarm_movement.cpp
