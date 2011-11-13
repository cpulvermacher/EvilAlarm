import Qt 4.7

Rectangle {
    width: 168
    height: 123
    color: "#00000000"
    Image{
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
        source: "TimeDisplayCorner.svg"
        sourceSize.width: 240
        sourceSize.height: 150
        width: 240;
        height: 150;
        Image{
            source:"bell.png"
            x:160
            y:67
        }
    }
}
