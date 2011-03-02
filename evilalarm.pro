TEMPLATE = app
TARGET = evilalarm
QT += maemo5 dbus phonon
CONFIG += silent release

OBJECTS_DIR = $${PWD}/tmp
MOC_DIR = $${PWD}/tmp
VPATH = $${PWD}/src $${PWD}/src/modules
INCLUDEPATH += $${PWD}/src
INCLUDEPATH += $${PWD}/src/modules

HEADERS += backend.h settings.h alarm.h daemon.h mainwindow.h alarmhistoryitem.h\
	module_list.h module_settings.h\
	alarm_blubbels.h blubbels.h alarm_blubbels_settings.h\
	alarm_movement.h alarm_movement_settings.h accelerometer.h
SOURCES += main.cpp backend.cpp settings.cpp alarm.cpp daemon.cpp mainwindow.cpp alarmhistoryitem.cpp\
	module_list.cpp\
	alarm_blubbels.cpp blubbels.cpp alarm_blubbels_settings.cpp\
	alarm_movement.cpp alarm_movement_settings.cpp accelerometer.cpp
