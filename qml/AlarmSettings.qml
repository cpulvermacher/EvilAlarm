import Qt 4.7

Image {
    id: alarmSettingsObject
    width: 800; height: 424
    y: 1
    source: "plain-bg.png"

    property alias alarmHour: spinnerHour.currentIndex
    property alias alarmMinute: spinnerMinute.currentIndex
    property alias alarmOn: alarmSwitch.on

    property int alarmTimeMinutes: -1; //hold current alarm time as 60*h+m, or -1 if none is set

    Component.onCompleted: {
        non_user_action = true;
        setUiAlarm(ui_alarm_hours, ui_alarm_minutes, ui_alarm_active);
        non_user_action = false;

        window.setUiAlarm.connect(alarmSettingsObject.setUiAlarm)
    }


    function setUiAlarm(hours, minutes, on) {
        alarmHour = hours;
        alarmMinute = minutes;
        alarmOn = on;
    }


    function updateUntilAlarm() {
        if(alarmOn){
            var currentTime = new Date;
            var hours = currentTime.getHours()
            var minutes = currentTime.getMinutes()
            var seconds = currentTime.getSeconds();

            var alarmTime = new Date;
            alarmTime.setHours(alarmHour, alarmMinute, 0, 0);
            if(hours*60 + minutes >= alarmHour*60 + alarmMinute) { //alarm tomorrow
                alarmTime.setDate(alarmTime.getDate() + 1);
            }
            if(alarmHour != alarmTime.getHours()) {
                //invalid alarm time set (i.e. missing hour when switching to DST), force selection to corrected value
                non_user_action = true;
                alarmHour = alarmTime.getHours();
                non_user_action = false;
            }
            var millisecondsUntilAlarm = alarmTime - currentTime;

            var hoursLeft = Math.floor(millisecondsUntilAlarm / (1000 * 3600));
            var minutesLeft = Math.floor(millisecondsUntilAlarm / (1000 * 60)) % 60;
            untilAlarm.text = "Until alarm:\n"+hoursLeft+" hours "+minutesLeft+" minutes"

            if(!spinnerHour.moving && !spinnerMinute.moving //spinners have stopped
            && alarmTimeMinutes != alarmHour*60+alarmMinute) { //and alarm isn't set yet
                if(!non_user_action) {
                    window.setAlarm(alarmHour, alarmMinute);
                }
                alarmTimeMinutes = alarmHour*60 + alarmMinute;

                //set timer to turn alarm off again
                alarmExpiredTimer.interval = millisecondsUntilAlarm;
                alarmExpiredTimer.start();

                updateUntilAlarmTimer.stop();
            }
        } else {
            untilAlarm.text="";

            if(!non_user_action) {
                //only stop alarm if we know it's running
                if(alarmTimeMinutes != -1) {
                    alarmTimeMinutes = -1;
                    window.unsetAlarm()
                }

                alarmExpiredTimer.stop();
            }
        }
    }

    //turn alarm state off again
    function alarmExpired() {
        non_user_action = true;
        alarmTimeMinutes = -1;
        alarmOn = false;
        non_user_action = false;
    }


    //used for setting alarm after the spinners have stopped
    Timer {
        id: updateUntilAlarmTimer;
        interval: 500; running: false; repeat: true;
        onTriggered: updateUntilAlarm();
    }
    Timer {
        id: alarmExpiredTimer;
        running: false; repeat: false;
        onTriggered: alarmExpired();
    }


    AlarmSwitch{
        id: alarmSwitch
        on: false
        x: 534
        y: 12
        width: 248
        height: 223
        onOnChanged: {
           // console.log("alarmswitch toggled: " + on);
            updateUntilAlarm();
        }
    }

    AlarmSettingsCorner{
        id:alarmSettingsCorner
        z: 3
    }

    Item {
        x: -1
        y: -7
        width: 0; height: 0
        Text{
            id: buttonLabel2
            x: 70
            y: 18
            color: "#aaaaaa"
            text: "Alarm time:"
            font.bold: false
            font.pointSize: 35
            z:1;

            Image {
                x: -56
                y: 4
                width: 48
                height: 48
                smooth: true
                source: "bell.png"
            }
        }

        Column {
            y: 83; x: 21; spacing: 40

            Spinner {
                id: spinnerHour
                currentIndex: 0;
                focus: true
                model: 24
                delegate: Text { font.pixelSize: 40; text: index; height: parent.itemHeight+10; width: (index>9)?40:20; }

                onCurrentIndexChanged: {
                    if(!non_user_action) {
                        updateUntilAlarmTimer.start();
                    }
                }
            }
        }
        Column {
            y: 83; x: 278; spacing: 40
            Spinner {
                id: spinnerMinute
                currentIndex: 0;
                focus: true
                model: 60
                delegate: Text {
                    font.pixelSize: 40;
                    text: index;
                    height: parent.itemHeight+10;
                    width: (index>9)?40:20;
                }

                onCurrentIndexChanged: {
                    if(!non_user_action) {
                        updateUntilAlarmTimer.start();
                    }
                }
            }
        }
    }

    Text {
        id: untilAlarm
        x: 534
        y: 218
        color: "#777777"
        text: ""
        font.pixelSize: 28
    }

    Item {
        id: changeAlarmType
        x: -1
        y: 368
        width: 713
        height: 62

        MouseArea {
            id: historyarea
            onClicked: window.showAlarmHistory(alarmHour, alarmMinute);
            x: 21+15
            width: 240
            height: 62
            Text {
                id: text2
                color: "#777777"
                text: "Previous alarms >"
                style: Text.Raised
                anchors.fill: historyarea
                font.pixelSize: 28
                wrapMode: Text.NoWrap
                verticalAlignment: Text.AlignVCenter
            }
        }

        MouseArea {
            id: alarmtypearea
            onClicked: window.selectAlarmType()
            x: 278+15
            width: 244
            height: 62
            Text {
                id: alarmtypetext
                color: "#777777"
                text: ui_alarm_type + " alarm >"
                style: Text.Raised
                anchors.fill: alarmtypearea
                font.pixelSize: 28
                wrapMode: Text.NoWrap
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
