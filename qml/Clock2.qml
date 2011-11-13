/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

import Qt 4.7

Item {
    id: clock
    width: 320; height: 430

    property int hours
    property int minutes
    property int seconds

    property bool alarmOn
    property int alarmHours
    property int alarmMinutes

    onAlarmOnChanged: { alarmHand.visible = alarmOn; }
    Component.onCompleted: { timeChanged(); }
    onMinutesChanged: {
        non_user_action = true;
        alarmSettings.updateUntilAlarm();
        non_user_action = false;
    }

    function timeChanged() {
        var date = new Date;
        hours = date.getHours()
        minutes = date.getMinutes()
        seconds = date.getSeconds();

        currentTime.text = Qt.formatTime(date, Qt.SystemLocaleShortDate);

        //refresh time ~200ms after second actually changes to ensure regular updates
        tickTimer.interval =  1000 - date.getMilliseconds() + 200;
    }

    Timer {
        id: tickTimer;
        interval: 1000;
        running: displayOn; //defined in main.qml
        repeat: true;
        onTriggered: clock.timeChanged()
    }

    Image { id: background; source: "anvil-clock-small-sans-anvil.png"; }


    Image {
        x: 154.5; y: 164
        z:2
        source: "hour.svg"
        smooth: true
        transform: Rotation {
            id: hourRotation
            origin.x: 5; origin.y: 85;
            angle: (clock.hours * 30) + (clock.minutes * 0.5)
            Behavior on angle {
                SpringAnimation { spring: 15; damping: 6; modulus: 360 }
            }
        }
    }
    Image {
        x: 157.5; y: 170
        z:3;
        source: "alarmHand.png"
        smooth: true
        visible:false
        id: alarmHand
        transform: Rotation {
            id: alarmHourRotation
            origin.x: 2.5; origin.y: 80;
            angle: (clock.alarmHours * 30) + (clock.alarmMinutes* 0.5)
            Behavior on angle {
                SpringAnimation { spring: 15; damping: 6; modulus: 360 }
            }
        }
    }

    Image {
        x: 156.5; y: 141
        source: "minute.svg"
        z:2
        smooth: true
        transform: Rotation {
            id: minuteRotation
            origin.x: 5; origin.y: 107;
            angle: clock.minutes * 6
            Behavior on angle {
                SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
            }
        }
    }

    Image {
        x: 157.5; y: 170
        fillMode: Image.PreserveAspectFit
        source: "second.png"
        smooth: true
        transform: Rotation {
            id: secondRotation
            origin.x: 2.5; origin.y: 80;
            angle: clock.seconds * 6
            Behavior on angle {
                SpringAnimation { spring: 70; damping: 0.6; modulus: 360 }
            }
        }
    }
}
