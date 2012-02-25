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
    id: toggleswitch
    width: background.width; height: background.height

    property bool on: false
    state: "off"
    onOnChanged: { state = on?"on":"off"; }


    function toggle() { on = !on; }

    function releaseSwitch() {
        if (knob.x == 118) {
            if (toggleswitch.state == "off") return;
        }
        if (knob.x == 1) {
            if (toggleswitch.state == "on") return;
        }
        toggle();
    }

    Image {
        id: background
        source: "switch-background.png"
        sourceSize.width: 247;
        sourceSize.height: 129;

        MouseArea {
            anchors.fill: parent; onClicked: toggle()

            Text {
                id: text1
                x: 26
                y: 40
                color: "#3e3d3d"
                text: "OFF"
                font.pixelSize: 41
            }

            Image {
                id: image1
                x: 147
                y: 9
                fillMode: Image.PreserveAspectFit
                z: 92
                source: "Wakedo.png"

            }

            Text {
                id: text2
                x: 152
                y: 60
                color: "#b5b5b5"
                text: "ON"
                visible: true
                font.pixelSize: 41
            }
        }
    }

    Image {
        id: knob
        x: 118; y: 2
        smooth: false
        visible: true
        source: "metalball2.png"

        MouseArea {
            smooth: false
            visible: true
            anchors.fill: parent
            drag.target: knob; drag.axis: Drag.XAxis; drag.minimumX: 1; drag.maximumX: 118
            onClicked: toggle()
            onReleased: releaseSwitch()
        }
    }

    states: [
        State {
            name: "on"
            PropertyChanges { target: knob; x: 1 }
            PropertyChanges { target: toggleswitch; on: true }
        },
        State {
            name: "off"
            PropertyChanges { target: knob; x: 118 }
            PropertyChanges { target: toggleswitch; on: false }
        }
    ]

    transitions: Transition {
        NumberAnimation { properties: "x"; easing.type: Easing.InOutQuad; duration: 200 }
    }
}
