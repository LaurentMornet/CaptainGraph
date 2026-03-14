OBJECTS_DIR = obj
MOC_DIR     = moc
CONFIG      += console

SOURCES      = src/main.cpp
HEADERS      = src/main.h
#
# Common
#
SOURCES     += src/common/manager.cpp
SOURCES     += src/common/configuration.cpp
SOURCES     += src/common/layout.cpp
SOURCES     += src/common/graph.cpp
SOURCES     += src/common/graph_stack.cpp
SOURCES     += src/common/plot.cpp
SOURCES     += src/common/plot_database.cpp

HEADERS     += src/common/manager.h
HEADERS     += src/common/configuration.h
HEADERS     += src/common/layout.h
HEADERS     += src/common/graph.h
HEADERS     += src/common/graph_stack.h
HEADERS     += src/common/plot.h
#
# ADAPTERS
#
SOURCES     += src/adapter/layout_adapter.cpp
SOURCES     += src/adapter/graph_adapter.cpp
SOURCES     += src/adapter/plot_adapter.cpp
SOURCES     += src/adapter/style_adapter.cpp
HEADERS     += src/adapter/layout_adapter.h
HEADERS     += src/adapter/graph_adapter.h
HEADERS     += src/adapter/plot_adapter.h
HEADERS     += src/adapter/style_adapter.h
#
# IHM
#
SOURCES     += src/ihm/ihm_tools.cpp
SOURCES     += src/ihm/main_window.cpp
SOURCES     += src/ihm/layout_frame.cpp
SOURCES     += src/ihm/graph_frame.cpp
SOURCES     += src/ihm/plot_frame.cpp
#SOURCES     += src/ihm/custom_widgets/auto_lineedit.cpp
SOURCES     += src/ihm/custom_widgets/chip_widget.cpp
SOURCES     += src/ihm/custom_widgets/chips_container.cpp
SOURCES     += src/ihm/custom_widgets/flow_layout.cpp
HEADERS     += src/ihm/ihm_tools.h
HEADERS     += src/ihm/main_window.h
HEADERS     += src/ihm/layout_frame.h
HEADERS     += src/ihm/graph_frame.h
HEADERS     += src/ihm/plot_frame.h
#HEADERS     += src/ihm/custom_widgets/auto_lineedit.h
HEADERS     += src/ihm/custom_widgets/chip_widget.h
HEADERS     += src/ihm/custom_widgets/chips_container.h
HEADERS     += src/ihm/custom_widgets/flow_layout.h

#
# TinyXml
#
INCLUDEPATH += lib/tinyxml2
SOURCES     += lib/tinyxml2/tinyxml2.cpp
HEADERS     += lib/tinyxml2/tinyxml2.h

RESOURCES   += resources/resources.qrc

QT          += core gui widgets
