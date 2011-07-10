import QtQuick 1.0

Rectangle {
    width: 420
    height: 200
    color: "#00000000"
    property alias alarmTime: alarmText
    property alias alarmTimeText: alarmText.text
    property alias alarm: alarmBox
    property alias noAlarm: noAlarmText
    property alias currentTime: currentTime

    Text {
        id: currentTime
        x: 10
        y: 0
        //anchors.horizontalCenter: parent.horizontalCenter
        color: "#dddddd"
        font.family: "Nokia Sans"
        font.pixelSize: 100
        style: Text.Raised; styleColor: "grey"
    }
    Text {
        id: noAlarmText
        x: 15
        y: 130
        text: ""
        visible: true
        //anchors.horizontalCenter: parent.horizontalCenter
        color: "#555"
        font.family: "Nokia Sans"
        font.pixelSize: 30
        //style: Text.Raised; styleColor: "white"
    }

    Rectangle {
        id: alarmBox
        x: 15
        y: 113
        width: 370
        height: 64
        color: "#00000000"
        visible: false

        Image {
            id: image2
            x: 0
            y: 0
            source: "Wakedo.png"
        }

        Text {
            id: alarmText
            x: 80
            y: 17
            color: "#c4c4c4"
            text: "Alarm at"
            visible: true
            font.pixelSize: 30
            font.family: "Nokia Sans"
        }

    }

}
