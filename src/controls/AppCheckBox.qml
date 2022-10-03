import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

ColumnLayout{
    property string labelInfo: ""
    property alias checkbox : control

    CheckBox{
        id: control
    }
    Label{
        text:labelInfo
        color:"gray"
        font.pointSize: 10
    }
}
