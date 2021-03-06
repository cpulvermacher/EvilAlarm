import Qt 4.7

Rectangle{
    color: "#141518"
    width: 800; height: 424
    id: window
    signal selectAlarmType()
    signal showAlarmHistory(int hours, int minutes)
    signal setAlarm(int hours, int minutes)
    signal unsetAlarm()

    //triggered from C++ to set QML state (e.g. from alarm history)
    signal setUiAlarm(int hours, int minutes, bool on)

    //used by Clock2 to stop updates when display is off
    property bool displayOn: true

    //when true, changes to variables/UI state do not trigger any signals like setAlarm()
    //this is useful for initalization or changes originating from the C++ side of the application
    property bool non_user_action: false;

    //used to change the alarm type in UI
    property string ui_alarm_type: ""

    //alarm state at startup time
    //-1 as a start value so changing them is effective even when something like 0:0 is set
    property int ui_alarm_hours: -1
    property int ui_alarm_minutes: -1
    property bool ui_alarm_active: false //updates UI [remaining time] & starts necessary timers, so change the others first

    Wakedo {
        anchors.centerIn: parent
    }
}
