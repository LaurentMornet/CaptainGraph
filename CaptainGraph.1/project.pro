OBJECTS_DIR = obj
MOC_DIR     = moc

INCLUDEPATH += lib/tinyxml2

SOURCES      = src/common/graph.cpp
SOURCES     += src/common/plot.cpp
SOURCES     += src/common/plot_registry.cpp
SOURCES     += src/adapters/plot_adapter.cpp
SOURCES     += src/ihm/ihm_tools.cpp
SOURCES     += src/ihm/undo_manager.cpp
SOURCES     += src/ihm/main_window.cpp
SOURCES     += src/ihm/graph_config_frame.cpp
SOURCES     += src/ihm/plot_config_frame.cpp

SOURCES     += lib/tinyxml2/tinyxml2.cpp

HEADERS      = src/common/graph.h
HEADERS     += src/common/plot.h
HEADERS     += src/adapters/plot_adapter.h
HEADERS     += src/ihm/ihm_tools.h
HEADERS     += src/ihm/undo_manager.h
HEADERS     += src/ihm/main_window.h
SOURCES     += src/ihm/graph_config_frame.h
HEADERS     += src/ihm/plot_config_frame.h

HEADERS     += lib/tinyxml2/tinyxml2.h

RESOURCES   += resources/resources.qrc

QT          += core gui widgets
