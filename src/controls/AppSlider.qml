import QtQuick 2.15
import QtQuick.Controls 2.15

Slider{
    id: slider
    property bool displayFloat:false
    property real fontSize:12

    function formatText()
    {
        if(displayFloat)
            sliderText.text = value.toFixed(1)
        else
            sliderText.text = parseInt(value)
    }

    onValueChanged: {
        formatText();
    }
    Component.onCompleted: {
        formatText();
    }
    Text{
        id: sliderText
        anchors.left : slider.right
        anchors.leftMargin: 5
        topPadding: 5
        font.pointSize:fontSize
    }
}
