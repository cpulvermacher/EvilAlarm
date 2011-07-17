import QtQuick 1.0

Rectangle {
    width: 168
    height: 123
    color: "#00000000"
    MouseArea {
        onClicked: wakedo.flipped = !wakedo.flipped
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
        width:240;
        height:150;
    }
    Image{
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
        width:240;
        height:150;
        Image{
            source:"bell.png"
            x:160
            y:67
        }
        source: "TimeDisplayCorner.svg"
    }
}
