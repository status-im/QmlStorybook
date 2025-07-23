import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Flow {
    id: root

    property alias model: repeater.model

    property string selection: model ? model[initialSelection] : ""
    property int initialSelection: 0

    Repeater {
        id: repeater

        RadioButton {
            text: modelData
            checked: root.initialSelection === index
            onToggled: selection = modelData
        }
    }
}
