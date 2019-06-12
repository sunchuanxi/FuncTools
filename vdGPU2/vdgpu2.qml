import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2 as NewControl
//import com.zigaa.data 1.0

Item {
    //enabled: accountData.currentGroup >= AccountData.Technician

    implicitWidth: childrenRect.width
    implicitHeight: childrenRect.height+10
    property bool firstInit: true
    property int spnboxWidth: 85
    property int lblWidth: 40

    Component.onCompleted: {
        calibration_type.currentIndex = jobClient.queryValue("calibration_type")
        edgePort.currentIndex = jobClient.queryValue("edgePort")
        scratchPort.currentIndex = jobClient.queryValue("scratchPort")
        mudgePort.currentIndex = jobClient.queryValue("mudgePort")

        dirtyArea.value = jobClient.queryValue("dirtyArea")
        dirtyThresh.value = jobClient.queryValue("dirtyThresh")
        losingAngle.value = jobClient.queryValue("losingAngle")
        scratchLength.value = jobClient.queryValue("scratchLength")

        firstInit = false
    }

    ColumnLayout
    {
        NewControl.TabBar
        {
            id: tabBar
            Layout.preferredWidth: 300
            currentIndex: 1
            //font.pixelSize: 20
            font.bold: true
            NewControl.TabButton
            {
                text: qsTr("rdoSetting")
            }
            NewControl.TabButton
            {
                text: qsTr("rdoInspect")
            }
        }

        StackLayout
        {
            id:stckLayout
            width: parent.width
            anchors.top: tabBar.bottom
            anchors.topMargin: 5
            currentIndex: tabBar.currentIndex

            Column
            {
                width: 320
                x:0
                Label
                {
                    text: (" ")
                }
                GroupBox
                {
                    width: 320
                    flat: true

                    GridLayout
                    {
                        columns: 5
                        width: 308
                        columnSpacing: 10
                        CheckBox
                        {
                            id: iscalibration_type
                        }
                        Label {
                            Layout.preferredWidth: lblWidth
                            enabled: iscalibration_type.checked
                            text: qsTr("calibration_type") }
                        ComboBox {
                            id: calibration_type
                            enabled: iscalibration_type.checked
                            Layout.preferredWidth: spnboxWidth
                            model: [ qsTr("camera1"),qsTr("camera2"),qsTr("camera3"),qsTr("camera4"),qsTr("camera5"),qsTr("camera6"),qsTr("camera7"),qsTr("camera8"),qsTr("camera9")]
                            onCurrentIndexChanged:
                            {
                                if (!firstInit) {
								    //jobClient.updateValue("calibration", true, true)
                                    jobClient.updateValue("calibration_type", currentIndex);
                                }
                            }
                        }
                    }
                }
            }


            Column
            {

                RowLayout{
                    CheckBox
                    {
                        id: isEdge
                    }
                    Label {
                        Layout.preferredWidth: lblWidth
                        enabled: isEdge.checked
                        text: qsTr("isEdge") }
                    ComboBox {
                        id: edgePort
                        enabled: isEdge.checked
                        Layout.preferredWidth: spnboxWidth
                        model: [ qsTr("port1"),qsTr("port2"),qsTr("port3"),qsTr("port4"),qsTr("port5"),qsTr("port6"),qsTr("port7"),qsTr("port8"),qsTr("port9")]
                        onCurrentIndexChanged:
                        {
                            if (!firstInit) {
                                jobClient.updateValue("edgePort", currentIndex);
                            }
                        }
                    }
                }

                Label{text: " "}

                RowLayout{
                    enabled: isEdge.checked
                    Label{
                        text: " "
                        Layout.preferredWidth: 17
                    }
                    Label
                    {
                        Layout.preferredWidth:  lblWidth
                        text: qsTr("losingAngle")
                    }
                    SpinBox
                    {
                        id: losingAngle
                        decimals: 3
                        Layout.preferredWidth: spnboxWidth
                        minimumValue: 0
                        maximumValue: 9999
                        stepSize: 0.01
                        onValueChanged: if (!firstInit) jobClient.updateValue("losingAngle", value)
                    }
                }

                Label{text: " "}

                RowLayout{
                    CheckBox
                    {
                        id: isScratch
                    }
                    Label {
                        Layout.preferredWidth: lblWidth
                        enabled: isScratch.checked
                        text: qsTr("isScratch") }
                    ComboBox {
                        id: scratchPort
                        enabled: isScratch.checked
                        Layout.preferredWidth: spnboxWidth
                        model: [ qsTr("port1"),qsTr("port2"),qsTr("port3"),qsTr("port4"),qsTr("port5"),qsTr("port6"),qsTr("port7"),qsTr("port8"),qsTr("port9")]
                        onCurrentIndexChanged:
                        {
                            if (!firstInit) {
                                jobClient.updateValue("scratchPort", currentIndex);
                            }
                        }
                    }
                }

                Label{text: " "}

                RowLayout{
                    enabled: isScratch.checked
                    Label{
                        text: " "
                        Layout.preferredWidth: 17
                    }
                    Label
                    {
                        Layout.preferredWidth:  lblWidth
                        text: qsTr("scratchLength")
                    }
                    SpinBox
                    {
                        id: scratchLength
                        decimals: 3
                        Layout.preferredWidth: spnboxWidth
                        minimumValue: 0
                        maximumValue: 9999
                        stepSize: 0.01
                        onValueChanged: if (!firstInit) jobClient.updateValue("scratchLength", value)
                    }
                }


                Label{text: " "}

                RowLayout{
                    CheckBox
                    {
                        id: isMudge
                    }
                    Label {
                        Layout.preferredWidth: lblWidth
                        enabled: isMudge.checked
                        text: qsTr("isMudge") }
                    ComboBox {
                        id: mudgePort
                        enabled: isMudge.checked
                        Layout.preferredWidth: spnboxWidth
                        model: [ qsTr("port1"),qsTr("port2"),qsTr("port3"),qsTr("port4"),qsTr("port5"),qsTr("port6"),qsTr("port7"),qsTr("port8"),qsTr("port9")]
                        onCurrentIndexChanged:
                        {
                            if (!firstInit) {
                                jobClient.updateValue("mudgePort", currentIndex);
                            }
                        }
                    }
                }

                Label{text: " "}

                RowLayout{
                    enabled: isMudge.checked
                    Label{
                        text: " "
                        Layout.preferredWidth: 17
                    }
                    Label
                    {
                        Layout.preferredWidth:  lblWidth
                        text: qsTr("dirtyArea")
                    }
                    SpinBox
                    {
                        id: dirtyArea
                        decimals: 3
                        Layout.preferredWidth: spnboxWidth
                        minimumValue: 0
                        maximumValue: 9999
                        stepSize: 0.01
                        onValueChanged: if (!firstInit) jobClient.updateValue("dirtyArea", value)
                    }
                    Label
                    {
                        Layout.preferredWidth:  lblWidth
                        text: qsTr("dirtyThresh")
                    }
                    SpinBox
                    {
                        id: dirtyThresh
                        decimals: 3
                        Layout.preferredWidth: spnboxWidth
                        minimumValue: 0
                        maximumValue: 9999
                        stepSize: 0.01
                        onValueChanged: if (!firstInit) jobClient.updateValue("dirtyThresh", value)
                    }
                }

            }

        }
    }

}
