TEMPLATE = app
TARGET = evilalarm
QT += maemo5 dbus phonon declarative
CONFIG += silent release

DEFINES += EVILALARM

OBJECTS_DIR = $${PWD}/tmp
MOC_DIR = $${PWD}/tmp
UI_DIR = $${PWD}/tmp
VPATH = $${PWD}/src $${PWD}/src/modules
INCLUDEPATH += $${PWD}/src
INCLUDEPATH += $${PWD}/src/modules

HEADERS += backend.h settings.h alarm.h daemon.h alarmhistoryitem.h\
	module_list.h module_settings.h\
	alarm_blubbels.h blubbels.h alarm_blubbels_settings.h\
	alarm_movement.h alarm_movement_settings.h accelerometer.h \
    settings2.h \
    Wakedo/about.h \
    Wakedo/alarmhistory.h
SOURCES += main.cpp backend.cpp settings.cpp alarm.cpp daemon.cpp alarmhistoryitem.cpp\
	module_list.cpp\
	alarm_blubbels.cpp blubbels.cpp alarm_blubbels_settings.cpp\
	alarm_movement.cpp alarm_movement_settings.cpp accelerometer.cpp \
    settings2.cpp \
    Wakedo/about.cpp \
    Wakedo/alarmhistory.cpp
SOURCES += \
    Wakedo/mainwindow.cpp \
    Wakedo/selectalarmtype.cpp
HEADERS += \
    Wakedo/mainwindow.h \
    Wakedo/selectalarmtype.h

FORMS += \
    Wakedo/mainwindow.ui \
    Wakedo/selectalarmtype.ui \
    settings2.ui \
    Wakedo/about.ui \
    Wakedo/alarmhistory.ui

maemo5 {
    target.path = /opt/evilalarm/bin
    INSTALLS += target
}

OTHER_FILES += \
    Wakedo/qml/Wakedo/main.qml \
    Wakedo/qml/Wakedo/AlarmSwitch.qml \
    Wakedo/qml/Wakedo/TimeDisplayCorner.qml \
    Wakedo/qml/Wakedo/AlarmSettingsCorner.qml \
    Wakedo/qml/Wakedo/TimeText.qml \
    Wakedo/qml/Wakedo/TimeDisplay.qml \
    Wakedo/qml/Wakedo/Spinner.qml \
    Wakedo/qml/Wakedo/Wakedo.qml \
    Wakedo/qml/Wakedo/Switch.qml \
    Wakedo/qml/Wakedo/Clock.qml \
    Wakedo/qml/Wakedo/Clock2.qml \
    Wakedo/qml/Wakedo/AlarmSettings.qml

# Add more folders to ship with the application, here
folder_01.source = Wakedo/qml/Wakedo
folder_01.target = Wakedo/qml
DEPLOYMENTFOLDERS = folder_01 folder_02

include(Wakedo/qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()


# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =
