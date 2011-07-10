import QtQuick 1.0

Rectangle{
    color: "#141518"
    id: window
    signal selectAlarmType()

    Row {
        anchors.centerIn: parent; spacing: 0
        Wakedo {
            angle: 180; yAxis: 1 }
    }
}

