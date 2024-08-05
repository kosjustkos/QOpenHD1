import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Shapes 1.0
import OpenHD 1.0

BaseWidget {
    enum ShiftAxis {
        X,
        Y
    }

    id: ballisticWidget
    visible: settings.show_ballistic_widget
    widgetIdentifier: "ballistic_calculator_widget"
    isFullScreen: true
    disable_dragging: true

    function get_shift(axis) {
        var shifts = _ballisticController.getBallisticShifts();
        var x_shift_px = shifts[0];
        var y_shift_px = shifts[1];

        switch(axis) {
            case BallisticWidget.ShiftAxis.X:
                return x_shift_px;
            case BallisticWidget.ShiftAxis.Y:
                return y_shift_px;
        }
    }

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
        x: (ballisticWidget.parent.width - width) / 2 + get_shift(BallisticWidget.ShiftAxis.X);
        y: (ballisticWidget.parent.height - height) / 2 + get_shift(BallisticWidget.ShiftAxis.Y);        
    }
}
