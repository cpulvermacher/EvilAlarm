import Qt 4.7

Image{
    anchors.right: parent.right
    anchors.bottom: parent.bottom;
    source: "TimeDisplayCorner.png"
    width: 240;
    height: 150;
    Text {
        color: "#aaaaaa"
        text: "Ok!"
        z: 5
        anchors.rightMargin: 18
        anchors.bottomMargin: 14
        font.bold: false
        font.pixelSize: 36
        id: text1
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
    }

    MouseArea {
        onClicked: wakedo.flipped = !wakedo.flipped
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: -4
        anchors.rightMargin: -5
    }
}
