import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import KLib 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    ColumnLayout {
        anchors.fill: parent
        spacing: 4
        Text {
            text: qsTr("RR")
        }
        SchedulerLogView {
            log: rrExec.log
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        Text {
            text: qsTr("EDF")
        }
        SchedulerLogView {
            log: edfExec.log
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
