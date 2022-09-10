import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

ColumnLayout{
    property string labelText:"Label"
    property string labelInfo:"Info"

    Label{
        text: labelText
    }
    Label{
        text:labelInfo
        color:"gray"
        font.pointSize: 10
    }

}
