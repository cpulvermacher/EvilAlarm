import Qt 4.7

Flipable {
    id: wakedo
    width: 800; height: 424

    property int angle: 180
    property bool flipped: false

    function showAlarmTime() {
        if(alarmSettings.status != Loader.Ready)
            return;

        timeDisplay.clock.alarmHours=alarmSettings.item.alarmHour;
        timeDisplay.clock.alarmMinutes=alarmSettings.item.alarmMinute;

        timeDisplay.clock.alarmOn=alarmSettings.item.alarmOn;

        var date = new Date(0, 0, 0, alarmSettings.item.alarmHour, alarmSettings.item.alarmMinute);
        timeDisplay.alarmTimeText="Alarm at "+Qt.formatTime(date, Qt.SystemLocaleShortDate);

        non_user_action = true;
        alarmSettings.item.updateUntilAlarm()
        non_user_action = false;
    }


    front: TimeDisplay {
        id:timeDisplay
    }
    back: Loader {
        id: alarmSettings
        onLoaded: wakedo.showAlarmTime()
    }

    Timer {
        interval: 500;
        repeat: false;
        running: true;
        onTriggered: alarmSettings.source = "AlarmSettings.qml";
    }

    states: [State {
        name: "backstate"; when: wakedo.flipped
        PropertyChanges {
            target: rotation1;
            // this is negative to make rotation go reverse, i.e. to have paper fold approach user
            angle: -wakedo.angle }
    },State {
    name: "alarmOn"; when: alarmSettings.item.alarmOn
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
    name: "alarmOff"; when: !alarmSettings.item.alarmOn
        StateChangeScript {
            script: showAlarmTime();
        }
        PropertyChanges {
            target: timeDisplay.noAlarm;
            text: "No alarm"
            visible: true
        }
    }]
    transform: Rotation {
        id: rotation1; origin.x: wakedo.width / 2; origin.y: wakedo.height / 2
        axis.x: 0; axis.y: 1; axis.z: 0;
    }

    transitions: Transition {
        NumberAnimation { target: rotation1; properties: "angle"; duration: 250; }
    }
}
