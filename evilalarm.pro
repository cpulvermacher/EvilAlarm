TEMPLATE = app
TARGET = evilalarm-ui
QT += maemo5 dbus phonon declarative
CONFIG += silent release warn_on

QMAKE_CXXFLAGS_WARN_ON = -Wall -Wundef -Wextra

QMAKE_CFLAGS_RELEASE -= -O3
QMAKE_CFLAGS_RELEASE += -Os
QMAKE_CXXFLAGS_RELEASE -= -O3
QMAKE_CXXFLAGS_RELEASE += -Os

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
    alarm_normal.h\
    mainwindow.h \
    selectalarmtype.h \
    about.h \
    alarmhistory.h
SOURCES += main.cpp backend.cpp settings.cpp alarm.cpp daemon.cpp alarmhistoryitem.cpp\
    module_list.cpp\
    alarm_blubbels.cpp blubbels.cpp alarm_blubbels_settings.cpp\
    alarm_movement.cpp alarm_movement_settings.cpp accelerometer.cpp \
    alarm_normal.cpp \
    mainwindow.cpp \
    selectalarmtype.cpp \
    about.cpp \
    alarmhistory.cpp

FORMS += \
    mainwindow.ui \
    about.ui \
    settings.ui


#QtCreator stuff
maemo5 {
    target.path = /opt/evilalarm/bin
    INSTALLS += target
}

#files shown in project pane
OTHER_FILES += \
    qml/main.qml \
    qml/AlarmSwitch.qml \
    qml/TimeDisplayCorner.qml \
    qml/AlarmSettingsCorner.qml \
    qml/TimeText.qml \
    qml/TimeDisplay.qml \
    qml/Spinner.qml \
    qml/Wakedo.qml \
    qml/Switch.qml \
    qml/Clock2.qml \
    qml/AlarmSettings.qml

# Add more folders to ship with the application, here
folder_01.source = qml
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01 folder_02

include(Wakedo/qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()


# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =
