import QtQuick 1.0

Rectangle {
    color: "#222222"

    ListModel {
        id: widgetmodel
        ListElement {
            iColor: "red"
             iVisible: true
        }
        ListElement {
            iColor: "blue"
             iVisible: true
        }
        ListElement {
            iColor: "green"
             iVisible: true
        }
        ListElement {
             iColor: "yellow"
              iVisible: true
        }
        ListElement {
             iColor: "pink"
              iVisible: true
        }
        ListElement {
             iColor: "orange"
             iVisible: true
        }
        ListElement {
             iColor: "brown"
              iVisible: true
        }
        ListElement {
             iColor: "gray"
              iVisible: true
        }
        ListElement {
             iColor: "white"
              iVisible: true
        }
    }

    Component {
        id: widgetdelegate
        Item {
            width: grid.cellWidth;
            height: grid.cellHeight

            Rectangle {
                id: im
                state: "inactive"
                //anchors.centerIn: parent
                width: grid.cellWidth - 10
                height: grid.cellHeight - 10
                x : 10
                y : 10

                color: iColor
                visible: iVisible

                border.color: "white"
                border.width: 0

                states: [
                    State {
                        name: "inactive";
                        when: (grid.firstIndexDrag == -1) || (grid.firstIndexDrag == index)
                        PropertyChanges { target: im; border.width: 0}
                    }
                ]
            }

            states: [
                State {
                    name: "inDrag"
                    when: index == grid.firstIndexDrag

                    PropertyChanges { target: im; border.width: 5 }
                    PropertyChanges { target: im; parent: container }

                    PropertyChanges { target: im; anchors.centerIn: undefined }
                    PropertyChanges { target: im; x: coords.mouseX - im.width/2}
                    PropertyChanges { target: im; y: coords.mouseY - im.height/2}
                }
            ]
        }
    }

    GridView {
        property int firstIndexDrag: -1
        id: grid
        objectName: "grid"
        interactive: false // no flickable
        anchors.fill: parent
        cellWidth: parent.width / 10
        cellHeight: parent.height - 10
        x : 10
        y : 10

        model: widgetmodel
        delegate: widgetdelegate

        //
        Item {
            id: container
            anchors.fill: parent
        }

        MouseArea {
            id: coords
            anchors.fill: parent
            onReleased: {
                var newIndex = grid.indexAt(mouseX, mouseY);
                var oldIndex = grid.firstIndexDrag;

                if (grid.firstIndexDrag != -1 && newIndex != -1 && newIndex != oldIndex){
                    widgetmodel.move(oldIndex, newIndex, 1);
                    oldIndex = newIndex;
                }

                grid.firstIndexDrag = -1
            }

            onPressed: {
                grid.firstIndexDrag = grid.indexAt(mouseX, mouseY)
            }
        }
    }
}
