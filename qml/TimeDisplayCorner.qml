import Qt 4.7

Item {
    width: 168
    height: 123
    Image{
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
        source: "TimeDisplayCorner.png"
        width: 240;
        height: 150;
        Image{
            source:"bell.png"
            x:160
            y:67
        }
    }
}
