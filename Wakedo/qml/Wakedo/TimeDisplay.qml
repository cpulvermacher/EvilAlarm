import QtQuick 1.0
//import Qt.labs.gestures 1.0

Rectangle{
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
        MouseArea {
            onClicked: wakedo.flipped = !wakedo.flipped
            x:0;
            y:0;
            width:320;
            height:430;
            z: 0
        }
    }

    // none of this works on n900, will test once can get a hold on a better device
/*    GestureArea {
        anchors.fill: parent
        focus: true

        // Only some of the many gesture properties are shown. See Gesture documentation.

          onTap:
            noAlarmText.text="tap pos = ("+gesture.position.x+","+gesture.position.y+")";
        onTapAndHold:
            noAlarmText.text="tap and hold pos = ("+gesture.position.x+","+gesture.position.y+")";
        onPan:
            noAlarmText.text="pan delta = ("+gesture.delta.x+","+gesture.delta.y+") acceleration = "+gesture.acceleration;
        onPinch:
            noAlarmText.text="pinch center = ("+gesture.centerPoint.x+","+gesture.centerPoint.y+") rotation ="+gesture.rotationAngle+" scale ="+gesture.scaleFactor;
        onSwipe:
            noAlarmText.text="swipe angle="+gesture.swipeAngle
        onGesture:
            noAlarmText.text="gesture hot spot"+gesture.hotSpot.x+" "+gesture.hotSpot.y+")"
    }
    */

    TimeText{
        id:timeText
        x: 379
        y: 97
        width: 420
        height: 174
    }


    // usability test conclusion: clock picture
    // should also serve as trigger for setting alarm
    // it is an alarm clock so it is reasonable that users assume
    // it can be used to access alarm settings

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
