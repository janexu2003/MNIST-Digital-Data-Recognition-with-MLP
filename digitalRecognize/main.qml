import QtQuick 2.15
import QtQuick.Window 2.15
import Painter 1.0
import QtQuick.Controls 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Digit draw and recognision")
    color: "lightyellow"

    PainteritenQml{
        id: painterItem
        width:100
        height:100
        anchors.centerIn: parent
    }

    Column{
        anchors.left: painterItem.right
        anchors.leftMargin: 100
        anchors.verticalCenter: parent.verticalCental
        spacing: 10
        Button{
            text:"Clear"
            onClicked: {
                painterItem.clearImage(); //implement clearImage()
            }
        }

        Button{
            text:"Recognize"
            onClicked: {
               imageRecognizer.recognizeImage(painterItem.toQimage())
            }
        }
    }

    Text{
        anchors.top: painterItem.bottom
        anchors.topMargin: 50
        text:imageRecognizer.resultStr
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
