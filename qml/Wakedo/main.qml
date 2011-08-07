import QtQuick 1.0

Rectangle{
    color: "#141518"
    id: window
    signal selectAlarmType()
    signal showAlarmHistory(int hours, int minutes)
    signal setAlarm(int hours, int minutes)
    signal unsetAlarm()

    //used by Clock2 to stop updates when display is off
    property bool displayOn: true

    //used to change the alarm type in UI (which in turn will trigger setAlarm(), so this can't be used for initialization)
    property int ui_alarm_hours: 0
    property int ui_alarm_minutes: 0

    Row {
        anchors.centerIn: parent; spacing: 0
        Wakedo { angle: 180; yAxis: 1 }
    }
}
