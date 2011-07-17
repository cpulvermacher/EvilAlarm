TEMPLATE = app
TARGET = evilalarm
QT += maemo5 dbus phonon declarative
CONFIG += silent release

DEFINES += EVILALARM

OBJECTS_DIR = $${PWD}/tmp
MOC_DIR = $${PWD}/tmp
UI_DIR = $${PWD}/tmp
VPATH = $${PWD}/src $${PWD}/src/modules $${PWD}/forms
INCLUDEPATH += $${PWD}/src
INCLUDEPATH += $${PWD}/src/modules

HEADERS += backend.h settings.h alarm.h daemon.h alarmhistoryitem.h\
    module_list.h module_settings.h\
    alarm_blubbels.h blubbels.h alarm_blubbels_settings.h\
    alarm_movement.h alarm_movement_settings.h accelerometer.h \
    mainwindow.h \
    selectalarmtype.h \
    about.h \
    alarmhistory.h
SOURCES += main.cpp backend.cpp settings.cpp alarm.cpp daemon.cpp alarmhistoryitem.cpp\
    module_list.cpp\
    alarm_blubbels.cpp blubbels.cpp alarm_blubbels_settings.cpp\
    alarm_movement.cpp alarm_movement_settings.cpp accelerometer.cpp \
    mainwindow.cpp \
    selectalarmtype.cpp \
    about.cpp \
    alarmhistory.cpp \

FORMS += \
    mainwindow.ui \
    selectalarmtype.ui \
    about.ui \
    alarmhistory.ui \
    settings.ui

maemo5 {
    target.path = /opt/evilalarm/bin
    INSTALLS += target
}

OTHER_FILES += \
    qml/Wakedo/main.qml \
    qml/Wakedo/AlarmSwitch.qml \
    qml/Wakedo/TimeDisplayCorner.qml \
    qml/Wakedo/AlarmSettingsCorner.qml \
    qml/Wakedo/TimeText.qml \
    qml/Wakedo/TimeDisplay.qml \
    qml/Wakedo/Spinner.qml \
    qml/Wakedo/Wakedo.qml \
    qml/Wakedo/Switch.qml \
    qml/Wakedo/Clock.qml \
    qml/Wakedo/Clock2.qml \
    qml/Wakedo/AlarmSettings.qml

# Add more folders to ship with the application, here
folder_01.source = qml/Wakedo
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01 folder_02

include(Wakedo/qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()


# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =
