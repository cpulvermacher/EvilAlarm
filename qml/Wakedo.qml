import Qt 4.7

Flipable {
    id: wakedo

    width: 800; height: 424

    property int xAxis: 0
    property int yAxis: 0
    property int angle: 0
    property bool flipped: false

    function showAlarmTime() {
        timeDisplay.clock.alarmHours=alarmSettings.alarmHour;
        timeDisplay.clock.alarmMinutes=alarmSettings.alarmMinute;

        timeDisplay.clock.alarmOn=alarmSettings.alarmOn;

        var date = new Date(0, 0, 0, alarmSettings.alarmHour, alarmSettings.alarmMinute); 
        timeDisplay.alarmTimeText="Alarm at "+Qt.formatTime(date, Qt.SystemLocaleShortDate);
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
                    text: "No alarm"
                    visible: true
                }
            }]
    transform: Rotation {
        id: rotation1; origin.x: wakedo.width / 2; origin.y: wakedo.height / 2
        axis.x: wakedo.xAxis; axis.y: wakedo.yAxis; axis.z: 0;
    }

    transitions: Transition {
        NumberAnimation { target: rotation1; properties: "angle"; duration: 250; }
    }
}
