import QtQuick 2.15
import QtQuick.Controls 2.15

TextArea{
    id:textInput

    property int selectStart
    property int selectEnd
    property int curPos

    selectByMouse: true
    color: textInput.enabled ? "#ffffff": "gray"
    placeholderTextColor : "#8c8d8f"
    wrapMode: TextEdit.Wrap

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.IBeamCursor
        acceptedButtons: Qt.RightButton
        hoverEnabled: true

        onClicked: {
            textInput.selectStart = textInput.selectionStart;
            textInput.selectEnd = textInput.selectionEnd;
            textInput.curPos = textInput.cursorPosition;
            contextMenu.x = mouse.x;
            contextMenu.y = mouse.y;
            contextMenu.open();
            textInput.cursorPosition = textInput.curPos;
            textInput.select(textInput.selectStart,textInput.selectEnd)
        }

        onPressAndHold:  {
            if (mouse.source === Qt.MouseEventNotSynthesized) {
                selectStart = textInput.selectionStart;
                selectEnd = textInput.selectionEnd;
                curPos = textInput.cursorPosition;
                contextMenu.x = mouse.x;
                contextMenu.y = mouse.y;
                contextMenu.open();
                textInput.cursorPosition = curPos;
                textInput.select(selectStart,selectEnd);
            }
        }

    }

    Menu {
        id: contextMenu

        title: "Edit Context Menu"

        MenuItem {
            text: "Cut"
            onTriggered: {
                textInput.cut();
            }
        }

        MenuItem {
            text: "Copy"
            onTriggered: {
                textInput.copy();
            }
        }

        MenuItem {
            text: "Paste"
            onTriggered: {
                textInput.paste();
            }
        }
        MenuItem {
            text: "Select all"
            onTriggered: {
                textInput.selectAll();
            }
        }
    }
}
