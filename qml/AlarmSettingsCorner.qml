import Qt 4.7

Item {
    width: 173
    height: 150
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
    Image{
        id: image1
        anchors.right: parent.right
        anchors.bottom: parent.bottom;
        source: "TimeDisplayCorner.png"
        width: 240;
        height: 150;
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
