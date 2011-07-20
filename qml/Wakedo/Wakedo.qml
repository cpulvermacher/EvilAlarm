import QtQuick 1.0
//import "screenorientation.js" as ScreenOrientation

Flipable {
    id: wakedo

    width: 800; height: 430

    /*clip: true
    property variant selectedOrientation: Orientation.UnknownOrientation
    property variant activeOrientation: selectedOrientation == Orientation.UnknownOrientation ? runtime.orientation : selectedOrientation
    state: "orientation " + activeOrientation
    property bool inPortrait: (activeOrientation == Orientation.Portrait || activeOrientation == Orientation.PortraitInverted);

    // rotation correction for landscape devices like N900
    property bool landscapeWindow: window.width > window.height
    property variant rotationDelta: landscapeWindow ? -90 : 0
    rotation: rotationDelta

    // initial state is portrait
    property real baseWidth: landscapeWindow ? window.height-10 : window.width-10
    property real baseHeight: landscapeWindow ? window.width-10 : window.height-10

    width: baseWidth
    height: baseHeight
*/

    property int xAxis: 0
    property int yAxis: 0
    property int angle: 0
    property bool flipped: false

    function showAlarmTime() {
        timeDisplay.clock.alarmHours=alarmSettings.alarmHour;
        timeDisplay.clock.alarmMinutes=alarmSettings.alarmMinute;

        timeDisplay.clock.alarmOn=alarmSettings.alarmOn;

        var alarmMinutesPadded = ((alarmSettings.alarmMinute<10)?"0":"")+alarmSettings.alarmMinute;
        var alarmHoursPadded = ((alarmSettings.alarmHour<10)?"0":"")+alarmSettings.alarmHour;
        timeDisplay.alarmTimeText="Alarm at "+alarmHoursPadded+":"+alarmMinutesPadded

    }


    front: TimeDisplay{id:timeDisplay}
    back: AlarmSettings{id:alarmSettings}


    states: [State {
        name: "backstate"; when: wakedo.flipped
        PropertyChanges {
            target: rotation1;
            // this is negative to make rotation go reverse, i.e. to have paper fold approach user
            angle: -wakedo.angle }
    },State {
    name: "alarmOn"; when: alarmSettings.alarmOn
        StateChangeScript {
            script: showAlarmTime();
        }
        PropertyChanges {
            target: timeDisplay.alarm;
            visible:true
        }
        PropertyChanges {
            target: timeDisplay.alarmTime;
            visible:true
        }
        PropertyChanges {
            target: timeDisplay.noAlarm;
            visible:false
        }
    },State {
            name: "alarmOff"; when: !alarmSettings.alarmOn
                StateChangeScript {
                    script: showAlarmTime();
                }
                PropertyChanges {
                    target: timeDisplay.noAlarm;
                    // this is negative to make rotation go reverse, i.e. to have paper fold approach user
                    text: "No alarm"
                    visible:true
                }
            }]
    transform: Rotation {
        id: rotation1; origin.x: wakedo.width / 2; origin.y: wakedo.height / 2
        axis.x: wakedo.xAxis; axis.y: wakedo.yAxis; axis.z: 0;
    }

    transitions: Transition {
        NumberAnimation { target: rotation1; properties: "angle"; duration: 250; easing.type: Easing.OutInExpo; }
    }
}
