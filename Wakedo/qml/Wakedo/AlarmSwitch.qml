import QtQuick 1.0

Rectangle {
    width: 200
    height: 143
    color: "#00000000"
    x: 5
    y: 0
    property alias on: aswitch.on
    property alias aswitch: aswitch
    Text{
        id: buttonLabel
        color: "#aaaaaa"
        text: "Alarm:"
        font.bold: false
        font.pointSize: 35
        z:1;
    }
    Switch {
        id: aswitch
        x: 0
        y: 66
        width: 0
        height: 0
                on: false;
                z: 0
        }

}
