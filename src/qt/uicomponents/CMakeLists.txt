set(MODULE_NAME uicomponents)

set(MODULE_SOURCES
   ApplicationConfiguration.cpp
   ApplicationConfiguration.h
   VolumeControlHandler.cpp
   VolumeControlHandler.h
   MeterPanelHandler.cpp
   MeterPanelHandler.h
   IconCode.h
   TrackType.h
)

set(MODULE_QML_SOURCES
   qml/FlatButton.qml
   qml/LabelTrackControlPanel.qml
   qml/MeterPanel.qml
   qml/MonoTrackControlPanel.qml
   qml/PlayheadCursor.qml
   qml/PlayheadCursorHead.qml
   qml/StereoTrackControlPanel.qml
   qml/StyledDial.qml
   qml/StyledIconLabel.qml
   qml/StyledSlider.qml
   qml/StyledTextLabel.qml
   qml/TrackVolumeMeter.qml
   qml/ToolbarGrip.qml
   qml/ToolbarSeparator.qml
   qml/VideoTrackControlPanel.qml
   qml/VolumeControl.qml
   qml/VolumeControlTester.qml
)

qt_add_qml_module(${MODULE_NAME}
   URI "Audacity.UiComponents"
   VERSION 1.0
   STATIC
   RESOURCE_PREFIX /${MODULE_NAME}
   DEPENDENCIES
      QtQuick
   QML_FILES
      ${MODULE_QML_SOURCES}
   SOURCES
      ${MODULE_SOURCES}
   NO_LINT
)

organize_source( "${CMAKE_CURRENT_SOURCE_DIR}" "" "${MODULE_SOURCES}" )
organize_source( "${CMAKE_CURRENT_SOURCE_DIR}" "" "${MODULE_QML_SOURCES}" )
