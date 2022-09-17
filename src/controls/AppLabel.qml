import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

ColumnLayout{
    property string labelText: ""
    property string labelInfo: ""
    property string valueText: ""

    RowLayout{
        spacing: 10
        Layout.fillWidth: true
        Label{
            text: labelText
        }
        Label{
            text: valueText
        }
    }
    Label{
        text:labelInfo
        color:"gray"
        font.pointSize: 10
    }

}
