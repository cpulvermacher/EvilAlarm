import Qt 4.7

Item {
    width: 200
    height: 143
    x: 5
    y: 0
    property alias on: aswitch.on
    property alias aswitch: aswitch
    Text{
        id: buttonLabel
        x: 6
        y: 0
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
    }
}
