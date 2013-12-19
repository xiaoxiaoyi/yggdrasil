import QtQuick 1.0

Rectangle{
    id:mainLayout;
    objectName: "mainLayout";
    x: 0;
    y:0;
    width: 0;
    height: 0;
    property int oldWidth : 0;
    property int oldHeight : 0;

    Image {
        id: img
        objectName: "img";
        source: "../data/images/sprites/Arthur.png"
        x: 0;
        y: 0;
    }

    Rectangle {
        id: focus;
        objectName: "focus";

        property real ratio: 1.0;
        width: mainLayout.width;
        height: mainLayout.height;
        x:(mainLayout.width  - width )/2;
        y:(mainLayout.height - height)/2;
        color: "transparent";
        border.width: 2;
        border.color: "black";



        MouseArea {
            anchors.fill: parent;

            onClicked: {
                //parent.color = 'red';
            }
        }

        onRatioChanged: {
            if(ratio > 1.0){
                width = mainLayout.width;
                height = mainLayout.height / ratio;
            }
            else{
                width = mainLayout.width * ratio;
                height = mainLayout.height;
            }
        }
    }

    Text {
        id: bla
        text: qsTr("test")
    }

    MouseArea {
        id: coords;
        anchors.fill: parent;
        acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton;

        property int currentButton: 0;
        property real oldX;
        property real oldY;
        property real oldImageX;
        property real oldImageY;
        property real oldScale;

        onReleased: {
            currentButton = 0;
        }

        onPositionChanged: {
            if(currentButton == 1){
                img.x = oldImageX + (coords.mouseX - oldX);
                img.y = oldImageY + (coords.mouseY - oldY);
            }

            if(currentButton == 2){
                //img.scale = Math.min(Math.max(oldScale + (coords.mouseX - oldX)*oldScale/500, 1.0), 3.0);
                img.scale = oldScale + (coords.mouseX - oldX)*oldScale/500;
            }
        }

        onPressed: {
            focus.ratio = 16/9;
            if(pressedButtons & Qt.LeftButton){
                oldX = coords.mouseX;
                oldY = coords.mouseY;
                oldImageX = img.x;
                oldImageY = img.y;
                currentButton = 1;
            }

            if(pressedButtons & Qt.RightButton){
                oldX = coords.mouseX;
                oldY = coords.mouseY;
                oldScale = img.scale;
                currentButton = 2;
            }

            if(pressedButtons & Qt.MiddleButton)
                focus.border.color = "white";
        }
    }

    onWidthChanged: {
        if(oldWidth == 0){
            oldWidth = width;
            return;
        }

        var imgOldScale = img.scale;

        img.scale *= (width/oldWidth);

        img.x = img.x + ((img.width*img.scale - img.width*imgOldScale) / 2.0);
        img.y = img.y + ((img.height*img.scale - img.height*imgOldScale) / 2.0);
        //img.x += (img.x - imgX);

        bla.text = qsTr("x "+img.x+"y"+img.y+" scale"+x+"img x "+img.x+"img y "+y);

        oldWidth = width;

        //update ratio
        var ratio = focus.ratio;
        focus.ratio = 1;
        focus.ratio = ratio;
    }

    onHeightChanged: {
        if(oldHeight == 0){
            bla.text = qsTr("first init "+height)
            oldHeight = height;
            img.scale = 1.0;
            return;
        }

        /*
        bla.text = qsTr("oldHeight "+oldHeight+" Height"+height+
            "\n scale "+img.scale+
            "\n Img x "+img.x+" y "+img.y+
            "\n Img width "+img.width+" y "+img.height)
        */

        oldHeight = height;

        //update ratio
        var ratio = focus.ratio;
        focus.ratio = 1;
        focus.ratio = ratio;
    }
}


