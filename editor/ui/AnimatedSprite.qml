import QtQuick 1.0


Rectangle{
    id: login
    clip: true
    width: 80
    height: 25

    Image {
        id: loginButton
        x: -20
        y: 0
        source: "../../data/images/sprites/Arthur.png"
        SequentialAnimation {
            running: true;
            loops: Animation.Infinite
            NumberAnimation { target: loginButton; property: "x"; to: -50; duration: 1;}
            PauseAnimation  { duration: 1000;}
            NumberAnimation { target: loginButton; property: "y"; to: -50; duration: 1; }
            PauseAnimation  { duration: 1000;}
            NumberAnimation { target: loginButton; property: "y"; to: 0; duration: 1; }
        }
    }
}
