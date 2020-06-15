
import QtQuick 2.4
import QtQuick.Controls 2.0
import Qt3D.Animation 2.9
Item {


    Item {
        width: 1000
        height: 1000


        Rectangle {
            id: rectangle
            x: 25
            y: 165
            width: 909
            height: 655
            color: "#ffffff"

            Dial {
                id: dial
                x: 38
                y: 409
            }

            Button {
                id: button
                x: 159
                y: 119
                text: qsTr("Button")
            }

            ComboBox {
                id: comboBox
                x: 159
                y: 176
            }

            CheckBox {
                id: checkBox
                x: 159
                y: 229
                text: qsTr("Check Box")
            }

            TextField {
                id: textField
                x: 73
                y: 53
                text: qsTr("Text Field")
            }

            TextArea {
                id: textArea
                x: 46
                y: 155
                text: qsTr("Text Area")
            }

            RadioButton {
                id: radioButton
                x: 225
                y: 308
                text: qsTr("Radio Button")
            }

            RadioButton {
                id: radioButton1
                x: 225
                y: 354
                text: qsTr("Radio Button")
            }
        }
    }


}
