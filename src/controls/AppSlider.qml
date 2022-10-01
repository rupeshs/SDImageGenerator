import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

ColumnLayout{
    property alias header : headerLabel
    property alias description : info
    property alias slider : control
    property bool displayFloat: false
    property real fontSize: 12
    property real decimalPointNumbers : 1

    spacing: 10

    function formatText()
    {
        if(displayFloat)
            valueLabel.text = control.value.toFixed(decimalPointNumbers)
        else
            valueLabel.text = parseInt(control.value)
    }

    RowLayout{

        Label{
            id: headerLabel
        }
        Item{
            Layout.fillWidth: true
        }
        Label{
            id: valueLabel

        }
    }

    Label{
        id : info
        color:"gray"
        font.pointSize: 10
        Layout.fillWidth: true
        wrapMode: Label.WordWrap
    }

    Slider {
        id: control

        value: 0.5
        Layout.fillWidth: true
        background: Rectangle {
            x: control.leftPadding
            y: control.topPadding + control.availableHeight / 2 - height / 2
            implicitWidth: 200
            implicitHeight: 4
            width: control.availableWidth
            height: implicitHeight
            radius: 2
            color: "#1c1c1c"

            Rectangle {
                width: control.visualPosition * parent.width
                height: parent.height
                color: control.enabled? "#26a8ff" : "gray"
                radius: 2
            }
        }


        handle: Rectangle {
            id:mhandle
            x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
            y: control.topPadding + control.availableHeight / 2 - height / 2
            implicitWidth: 20
            implicitHeight: 20
            radius: 13
            scale: control.pressed ? 1.3 : 1

            Behavior on scale {
                NumberAnimation {
                    duration: 250
                }
            }

        }
        Rectangle {
            id : overlayhand
            x:mhandle.x
            y:mhandle.y
            implicitWidth: 30
            implicitHeight: 30
            radius: 20
            opacity:0.5
            anchors.centerIn: parent
            visible: false

        }

        onValueChanged: {
            formatText();
        }
        Component.onCompleted: {
            formatText();
        }
    }

}
