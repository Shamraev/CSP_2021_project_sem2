import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtCharts 2.3

Window {
    id: root
    visible: true
    width: 800
    height: 600
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("Controller")

    Rectangle {
        id: _background
        anchors.fill: parent
        anchors.margins: 2
    }

    Rectangle {
        id: _field
        property double scaleFactor: _field.height / 100

        anchors {
            top: _background.top
            bottom: _controlPanel.top
            left: _background.left
            right: _background.horizontalCenter
            margins: 2
        }

        height: root.width / 2
        width: root.width / 2
        border.color: "black"
        border.width: 5
        radius: 10

        Layout.column: 1
        Layout.row: 0

        Rectangle {
            id: _plant
            color: "blue"
            height: 20
            width: 100

            property double output: plant.output

            x: (_field.width  - _plant.width) / 2
            y: _field.height - _plant.height - _plant.output * _field.scaleFactor
        }

        Rectangle {
            id: _level
            property double referenceLevel: 0

            color: "red"
            height: 3
            y: _field.height - referenceLevel * _field.scaleFactor - 1

            anchors {
                left: _field.left
                right: _field.right
            }

            onReferenceLevelChanged: {
                plant.setReferenceSignal(referenceLevel)
            }

            Connections {
                target: plant

                function onReferenceSignalChanged(referenceSignal) {
                    _level.referenceLevel = referenceSignal;
                }
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            propagateComposedEvents: true
            hoverEnabled: true

            property bool holded: false

            onPressed: {
               applyMousePosition
               holded = true
            }

            onReleased: {
                holded = false
            }

            onDoubleClicked: {
                applyReferenceLevel(0)
            }

            onPositionChanged: {
                if (holded) {
                    applyMousePosition()
                }
            }

            function applyReferenceLevel(value) {
                var valueToSet = value

                if (valueToSet > 100) {
                    valueToSet = 100
                }
                if (valueToSet < 0) {
                    valueToSet = 0
                }
                _level.referenceLevel = valueToSet
            }

            function applyMousePosition() {
                applyReferenceLevel((_field.height -  mouseArea.mapToItem(_field, mouseArea.mouseX, mouseArea.mouseY).y) / _field.scaleFactor)
            }
        }
    }

    Rectangle {
        id: _controlPanel

        height: 50

        anchors {
            bottom: _background.bottom
            left: _background.left
            right: _background.right
        }

        Button {
            id: _startButton

            text: "Start"

            anchors {
                bottom: _controlPanel.bottom
                right: _stopButton.left
                top: _controlPanel.top
                margins: 5
            }

            onClicked: {
                _plantTrajectory.clear()
                _referenceTrajectory.clear()
                plant.start(_initialPosition.value);
            }
        }

        Button {
            id: _stopButton

            text: "Stop"

            anchors {
                bottom: _controlPanel.bottom
                right: _initialPosition.left
                top: _controlPanel.top
                margins: 5
            }

            onClicked: {
                plant.stop()
            }
        }

        SpinBox {
            id: _initialPosition

            anchors {
                bottom: _controlPanel.bottom
                right: _controlPanel.right
                top: _controlPanel.top
                margins: 5
            }

            from: 0
            to: 100
            value: 0
            editable: true
        }
    }

    ChartView {
        id: _chart

        anchors {
            top: _background.top
            bottom: _controlPanel.top
            right: _background.right
            left: _background.horizontalCenter
        }

        ValueAxis {
            id: _axisX
            min: 0
            max: 60
        }

        ValueAxis {
            id: _axisY
            min: 0
            max: 100
        }

        LineSeries {
            id: _plantTrajectory
            name: "Plant history"

            axisX: _axisX
            axisY: _axisY

            Connections {
                target: plant

                function onOutputChanged(output, seconds) {
                    _plantTrajectory.append(seconds, output)

                    if (seconds > _axisX.max) {
                        _axisX.max = Math.round(seconds) + 1
                    }
                }
            }
        }

        LineSeries {
            id: _referenceTrajectory
            name: "Reference history"

            axisX: _axisX
            axisY: _axisY

            Connections {
                target: plant

                function onInputChanged(output, seconds) {
                    _referenceTrajectory.append(seconds, output)

                    if (seconds > _axisX.max) {
                        _axisX.max = Math.round(seconds) + 1
                    }
                }
            }
        }

        LineSeries {
            id: _referenceLevel
            name: "Current reference"

            axisX: _axisX
            axisY: _axisY

            width: 2
            color: "blue"
            style: Qt.DashLine

            XYPoint {
                id: _leftPoint
                x: _axisX.min
                y: _level.referenceLevel
            }

            XYPoint {
                id: _rightPoint
                x: _axisX.max
                y: _level.referenceLevel
            }

            Connections {
                target: _level

                function onReferenceLevelChanged(referenceLevel) {
                    _referenceLevel.clear()
                    _referenceLevel.append(_axisX.min, _level.referenceLevel)
                    _referenceLevel.append(_axisX.max, _level.referenceLevel)
                }
            }
        }
    }
}
