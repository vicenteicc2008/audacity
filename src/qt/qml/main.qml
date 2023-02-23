import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Audacity
import Audacity.UiComponents

ApplicationWindow {
   id: root
   width: 1024
   height: 480
   visible: true
   title: qsTr("Audacity")
   minimumWidth: 480
   minimumHeight: 540

   required property ApplicationConfiguration appConfig
   property alias workspaceMode: toolsToolbar.workspaceMode
   property alias enableVolumeTester: toolsToolbar.enableVolumeTester

   QtObject {
      id: testers
      property bool volumeVisible: false
   }

   menuBar: MenuBar {
      Menu {
         title: qsTr("File")
         MenuItem {
            text: qsTr("Exit")
            onTriggered: Qt.quit()
         }
      }

      Menu {
         title: qsTr("Workspace")
         MenuItem {
            text: qsTr("Classic")
            autoExclusive: true
            checkable: true
            checked: root.workspaceMode === Workspace.Mode.Classic
            onTriggered: {
               workspaceMode = Workspace.Mode.Classic
            }
         }

         MenuItem {
            text: qsTr("Simple recording")
            autoExclusive: true
            checkable: true
            checked: root.workspaceMode === Workspace.Mode.SimpleRecording
            onTriggered: {
               workspaceMode = Workspace.Mode.SimpleRecording
            }
         }

         MenuItem {
            text: qsTr("Audio editing")
            autoExclusive: true
            checkable: true
            checked: root.workspaceMode === Workspace.Mode.AudioEditing
            onTriggered: {
               workspaceMode = Workspace.Mode.AudioEditing
            }
         }

         MenuItem {
            text: qsTr("Spectral editing")
            autoExclusive: true
            checkable: true
            checked: root.workspaceMode === Workspace.Mode.SpectralEditing
            onTriggered: {
               workspaceMode = Workspace.Mode.SpectralEditing
            }
         }
      }

      Menu {
         title: qsTr("Testers")
         MenuItem {
            text: qsTr("VolumeControl")
            autoExclusive: true
            checkable: true
            checked: enableVolumeTester
            onTriggered: {
               enableVolumeTester = !enableVolumeTester
            }
         }
      }
   }

   header: ToolsToolbar {
      id: toolsToolbar
      onUpdateStatusBar: function(status) {
         statusBar.text = status
      }
   }

   footer: Rectangle {
      width: parent.width
      height: 30
      color: appConfig.backgroundColor1

      Text {
         id: statusBar
         anchors.centerIn: parent
      }

      Rectangle {
         anchors.bottom: parent.top
         height: 1
         width: parent.width
         color: appConfig.strokeColor1
      }
   }
}
