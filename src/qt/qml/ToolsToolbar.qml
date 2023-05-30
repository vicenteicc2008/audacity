import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Audacity
import Audacity.UiComponents

Rectangle {
   id: root
   width: parent.width
   color: appConfig.backgroundColor1

   property var workspaceMode: Workspace.Mode.Classic
   property alias enableVolumeTester: masterVolumeToolbar.testerVisible

   signal playbackStarted()
   signal playbackStopped()
   signal playbackPaused()
   signal updateStatusBar(status: string)

   onWorkspaceModeChanged: {
      transportToolbar.workspaceMode = workspaceMode
      editToolbar.workspaceMode = workspaceMode
   }

   ToolsToolbarHandler {
      id: toolbarHandler

      onUpdateStatusBar: function(status) {
         root.updateStatusBar(status)
      }
   }

   Flow {
      id: flowId
      spacing: 0
      Layout.alignment: Qt.AlignVCenter
      anchors.fill: parent

      TransportToolbar {
         id: transportToolbar
         gripVisible: Positioner.isFirstItem
         separatorVisible: !Positioner.isLastItem

         onPlaybackStarted: {
            timeToolbar.start()
            root.playbackStarted()
            masterVolumeToolbar.startDemo()
         }

         onPlaybackStopped: {
            timeToolbar.stop()
            root.playbackStopped()
            masterVolumeToolbar.stopDemo()
         }

         onPlaybackPaused:  {
            timeToolbar.pause()
            root.playbackPaused()
            masterVolumeToolbar.pauseDemo()
         }

         onUpdateStatusBar: function(status) {
            root.updateStatusBar(status)
         }
      }

      EditToolbar {
         id: editToolbar
         gripVisible:  Positioner.isFirstItem
         separatorVisible: !Positioner.isLastItem

         onUpdateStatusBar: function(status) {
            root.updateStatusBar(status)
         }
      }

      TimeToolbar {
         id: timeToolbar
         gripVisible:  Positioner.isFirstItem
         separatorVisible: !Positioner.isLastItem

         onUpdateStatusBar: function(status) {
            root.updateStatusBar(status)
         }
      }

      MasterVolumeToolbar {
         id: masterVolumeToolbar
         gripVisible:  Positioner.isFirstItem
         separatorVisible: !Positioner.isLastItem

         onUpdateStatusBar: function(status) {
            root.updateStatusBar(status)
         }
      }

      FlatButton {
         id: setup
         x: root.width - setup.width - 12
         icon: IconCode.SETUP
         onClicked: toolbarHandler.Setup()

         // The following is a hack to vertical center this button within
         // current row of the flow control
         QtObject {
            id: prv
            property bool stopYUpdateCascade: false
         }

         onYChanged: {
            if (!prv.stopYUpdateCascade) {
               prv.stopYUpdateCascade = true
               root.height =  setup.y + 48
               setup.x += 8
               setup.y += 8
            } else {
               prv.stopYUpdateCascade = false
            }
         }
      }
   }

   Rectangle {
      anchors.top: root.bottom
      height: 1
      width: parent.width
      color: appConfig.strokeColor1
   }
}

