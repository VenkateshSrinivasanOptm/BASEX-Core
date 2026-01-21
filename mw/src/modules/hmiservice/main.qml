import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: root
    width: 800
    height: 480
    visible: true
    title: "BASEX Core"

    Rectangle {
        anchors.fill: parent
        color: "#0F172A"   // dark neutral background

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 16

            // Product Name
            Text {
                text: "BASEX Core"
                font.pixelSize: 36
                font.bold: true
                color: "#E5E7EB"
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }

            // Tagline
            Text {
                text: "Common Foundation Platform"
                font.pixelSize: 16
                color: "#9CA3AF"
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }

            Rectangle {
                width: 260
                height: 1
                color: "#374151"
                Layout.alignment: Qt.AlignHCenter
            }

            // System Status
            Text {
                text: "System Status: ACTIVE"
                font.pixelSize: 14
                color: "#22C55E"
                Layout.alignment: Qt.AlignHCenter
            }

            // Info block
            ColumnLayout {
                spacing: 6
                Layout.alignment: Qt.AlignHCenter

                Text {
                    text: "Core Services Initialized"
                    font.pixelSize: 13
                    color: "#D1D5DB"
                    Layout.alignment: Qt.AlignHCenter
                }

                Text {
                    text: "Ready for Product Integration"
                    font.pixelSize: 13
                    color: "#D1D5DB"
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }

        // Footer
        Text {
            text: "Powered by Optm Media Solutions Pvt Ltd"
            font.pixelSize: 12
            color: "#6B7280"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 12
        }
    }
}
