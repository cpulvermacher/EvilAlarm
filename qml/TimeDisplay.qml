import QtQuick 1.0

Rectangle{
    MouseArea {
        //make full area clickable, since there are no interactive elements anyway
        onClicked: wakedo.flipped = !wakedo.flipped
        x:0;
        y:0;
        width:800;
        height:430;
        z: 0
	  }
    id: rectangle1
    width: 800; height: 430
    property alias alarmTime: timeText.alarmTime
    property alias alarmTimeText: timeText.alarmTimeText
    property alias alarm: timeText.alarm
    property alias noAlarm: timeText.noAlarm
    property alias clock: clock
    property alias currentTime: timeText.currentTime

    //property alias alarmTimeVisible: timeText.visible
    gradient: Gradient {
        GradientStop {
            position: 0.00;
            color: "#4b4f59";
        }
        GradientStop {
            position: 0.35;
            color: "#232323";
        }
        GradientStop {
            position: 1.00;
            color: "#232323";
        }
    }
    Clock2{
        id:clock
        x: 30
        z: 3
    }

    TimeText {
        id:timeText
        x: 379
        y: 97
        width: 420
        height: 174
    }

    TimeDisplayCorner{
        id: timeDisplayCorner
        z: 3
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
    }

    Rectangle {
        id: changeAlarmType
        x: 0
        y: 368
        width: 713
        height: 62
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#19191c"
            }

            GradientStop {
                position: 0.91
                color: "#232323"
            }

            GradientStop {
                position: 1
                color: "#232323"
            }
        }
    }
}
