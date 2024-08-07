import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Shapes 1.0
import OpenHD 1.0

BaseWidget {
    id: ballisticWidget
    visible: settings.show_ballistic_widget
    widgetIdentifier: "ballistic_calculator_widget"
    isFullScreen: true
    disable_dragging: true

    Text {
        visible:true
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        id: ballistic_debug_text
        color: settings.color_text
        font.pixelSize: 12
        font.family: settings.font_text
        text: _ballisticController.getDebugInfo()
        style: Text.Outline
        styleColor: settings.color_glow
    }

    Rectangle {
        id: landingCircle
        width: 30
        height: 30
        color: "transparent"
        border.width: 4
        border.color: "green"
        radius: 15
        x: (ballisticWidget.parent.width - width) / 2 + _ballisticController.ballisticShifts[0];
        y: (ballisticWidget.parent.height - height) / 2 + _ballisticController.ballisticShifts[1];
    }
}
