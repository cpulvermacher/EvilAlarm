import QtQuick 1.0

Rectangle {
    id: rectangle1
    width: 173
    height: 150
    color: "#00000000"
    Text {
        color: "#aaaaaa"
        text: "Ok!"
        z: 5
        anchors.rightMargin: 11
        anchors.bottomMargin: 14
        font.bold: false
        font.pixelSize: 36
        id: text1
        x: 166
        y: 93
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
    }
    Image{
        id: image1
        x: 0
        height:150;
        anchors.right: parent.right
        anchors.rightMargin: 0
        source: "TimeDisplayCorner.svg"
        Image{
            source:"ok.svg"
            x:165
            y:95
            anchors.right: parent.right
            anchors.rightMargin: 9
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 15
            visible: false
        }
    }

    MouseArea {
        onClicked: wakedo.flipped = !wakedo.flipped
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: -4
        anchors.right: parent.right
        anchors.rightMargin: -5
    }

}
