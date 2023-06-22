set(MODULE_NAME numeric-formats)

set(MODULE_SOURCES
   NumericConverterFormatterAdapter.cpp
   NumericConverterFormatterAdapter.h
   NumericConverterFormatterAdapterFactory.cpp
   NumericConverterFormatterAdapterFactory.h
)

set(MODULE_QML_SOURCES
   qml/TimecodeControl.qml
)

qt_add_qml_module(${MODULE_NAME}
   URI "Audacity.NumericFormats"
   VERSION 1.0
   STATIC
   RESOURCE_PREFIX /${MODULE_NAME}
   DEPENDENCIES
      QtQuick
      Audacity.UiComponents
   QML_FILES
      ${MODULE_QML_SOURCES}
   SOURCES
      ${MODULE_SOURCES}
   NO_LINT
)

target_link_libraries(${MODULE_NAME}
   PRIVATE
      lib-project-interface
      lib-numeric-formats-interface
)

organize_source( "${CMAKE_CURRENT_SOURCE_DIR}" "" "${MODULE_SOURCES}" )
organize_source( "${CMAKE_CURRENT_SOURCE_DIR}" "" "${MODULE_QML_SOURCES}" )
