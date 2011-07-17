import QtQuick 1.0

Rectangle{
    id: alarmSettings

    property alias hour: spinnerHour.currentIndex
    property alias minute: spinnerMinute.currentIndex

    property alias alarmOn: alarmSwitch.on

    property string secondsPadded
    property string minutesPadded
    property string hoursPadded

    // time zone shift; not in use
    property real shift: 0

    property bool initialized: false;

    //initialize
    Component.onCompleted: {
        hour = evilalarm_hours;
        minute = evilalarm_minutes;
        alarmOn = evilalarm_active;
        initialized = true;
    }


// an attempt to have minutes padded with zeroes in minute spinner
/*    property int i;


    function initSpinner(){
        for(i=0;i<60;i++){
            if(i<10){
                i="0"+i
            }
            minutemodel.append(""+i)
        }
        return minutemodel
    }
*/


    function updateUntilAlarm() {
        if(alarmOn){
            var date = new Date;
            var hours = shift ? date.getUTCHours() + Math.floor(clock.shift) : date.getHours()
            var minutes = shift ? date.getUTCMinutes() + ((clock.shift % 1) * 60) : date.getMinutes()
            var seconds = date.getUTCSeconds();

            var totalMinutes = hours*60+minutes;
            var totalAlarmMinutes = hour*60+minute;
            var hoursLeft;
            var minutesLeft;
            if(totalMinutes > totalAlarmMinutes){
                // the alarm is tomorrow
                var minutesLeftToday = 60*24-totalMinutes
                hoursLeft=Math.floor((minutesLeftToday + totalAlarmMinutes)/60)
                minutesLeft=Math.floor((minutesLeftToday + totalAlarmMinutes)%60)
            }else{
                // the alarm is today
                hoursLeft=Math.floor((totalAlarmMinutes-totalMinutes)/60);
                minutesLeft=Math.floor((totalAlarmMinutes-totalMinutes)%60);
            }

            if(hoursLeft<10){
                hoursPadded="0"+hoursLeft
            }else{
                hoursPadded=hoursLeft
            }
            if(minutesLeft<10){
                minutesPadded="0"+minutesLeft
            }else{
                minutesPadded=minutesLeft
            }

            //timeText.text = hoursPadded +":"+ minutesPadded // SECONDS ARE DISTRACTING? +":"+ secondsPadded
            // the date was too much
            //dateText.text = date.getDate() +"."+  date.getMonth() +"."+  date.getFullYear();
            /// @todo get local date format
            //dateText.text = date.toDateString();

            untilAlarm.text = "Until alarm:\n"+hoursLeft+" hours "+minutesLeft+" minutes"

            window.setAlarm(hour, minute);

            //set timer to turn alarm off again
            var milisecondsUntilAlarm = 1000*60*(hoursLeft*60 + minutesLeft);
            alarmExpiredTimer.interval = milisecondsUntilAlarm;
            alarmExpiredTimer.start();
        }
        else{
            untilAlarm.text="";

            window.unsetAlarm()

            alarmExpiredTimer.stop();
        }
    }

    //turn alarm state off again
    function alarmExpired() {
        alarmOn = false;
        updateUntilAlarm();
    }



    Timer {
        id: updateUntilAlarmTimer;
        interval: 1000; running: false; repeat: false;
        onTriggered: updateUntilAlarm();
    }
    Timer {
        id: alarmExpiredTimer;
        running: false; repeat: false;
        onTriggered: alarmExpired();
    }


    width: 800; height: 430
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
    border.color: "black"

    AlarmSwitch{
        id: alarmSwitch
        x: 534
        y: 12
        width: 248
        height: 223
        onOnChanged: {
            updateUntilAlarm();
        }
    }

    AlarmSettingsCorner{
        id:alarmSettingsCorner
        z: 3
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
    }

    Rectangle {
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
                width: 50
                height: 48
                smooth: true
                source: "bell.png"
            }
        }

        Column {
            y: 83; x: 21; spacing: 40

            Spinner {
                id: spinnerHour
                width: 240; height: 275
                focus: true
                model: 24
                itemHeight: 52
                delegate: Text { font.pixelSize: 40; text: index; height: parent.itemHeight+10; width: (index>9)?40:20; }

                onCurrentIndexChanged: {
                    updateUntilAlarmTimer.start();
                    if(!alarmSwitch.on && initialized){
                        alarmSwitch.aswitch.toggle();
                    }
                }
            }

        }
        Column {
            y: 83; x: 278; spacing: 40
            Spinner {
                id: spinnerMinute
                width: 240; height: 275
                focus: true
                model: 60
                itemHeight: 52
                delegate: Text {
                    font.pixelSize: 40;
                    text: index;
                    //text: name;
                    height: parent.itemHeight+10;
                    width: (index>9)?40:20;
                }
                //the following works well otherwise but the spinner breaks:
                /*MouseArea {
                    anchors.fill: parent
                    onFocusChanged: {
                        alarm.knob.x=1
                        alarm.on=true
                    }
                }*/

                onCurrentIndexChanged: {
                    updateUntilAlarmTimer.start();
                    if(!alarmSwitch.on && initialized){
                        alarmSwitch.aswitch.toggle();
                    }
                }
            }
        }


    }
    ListModel { id: minuteModel
/*        function(){
                                    for(i=0;i<=60;i++){
                                        var name=""+i
                                        if(i<10){
                                            name="0"+name;
                                        }
                                        model.append({'name': name})

                                    }
                                    return model;
                                }*/}

    Text {
        id: untilAlarm
        x: 534
        y: 218
        color: "#555555"
        text: ""
        font.pixelSize: 28
    }

    Rectangle {
        id: changeAlarmType
        x: -1
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

        MouseArea {
            id: mouse_area1
            onClicked: window.selectAlarmType()
            x: 7
            y: 0
            width: 244
            height: 54
            transformOrigin: Item.TopLeft
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 462
            anchors.left: parent.left
            anchors.leftMargin: 7
            anchors.top: parent.top
            anchors.topMargin: 0

            Text {
                id: text1
                x: 1
                y: 12
                width: 239
                height: 44
                color: "#626262"
                text: "Normal alarm >"
                anchors.horizontalCenterOffset: -1
                font.italic: false
                style: Text.Raised
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenterOffset: 3
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.NoWrap
                font.pixelSize: 28
            }
        }

        MouseArea {
            id: mouse_area2
            x: 254
            y: 0
            onClicked: window.alarmHistory()
            width: 283
            height: 62
            anchors.verticalCenterOffset: 0
            anchors.verticalCenter: parent.verticalCenter
            Text {
                id: text2
                x: 58
                y: 13
                width: 168
                height: 62
                color: "#626262"
                text: "Previous alarms >"
                style: Text.Raised
                anchors.verticalCenterOffset: 13
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 28
                wrapMode: Text.NoWrap
                horizontalAlignment: Text.AlignHCenter
            }
            transformOrigin: Item.TopLeft
        }
    }
}
