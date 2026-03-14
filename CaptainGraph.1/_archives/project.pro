OBJECTS_DIR = obj
MOC_DIR     = moc

SOURCES      = src/main.cpp
SOURCES     += src/common/app_context.cpp
SOURCES     += src/common/metagraph.cpp
SOURCES     += src/ihm/main_window.cpp
SOURCES     += src/ihm/workspace_frame.cpp
SOURCES     += src/ihm/graph_container_frame.cpp
SOURCES     += src/ihm/graph_frame.cpp
SOURCES     += src/ihm/docks/fs_browser_frame.cpp
SOURCES     += src/ihm/docks/page_config_frame.cpp
SOURCES     += src/ihm/docks/graph_config_frame.cpp
SOURCES     += src/ihm/docks/plot_config_frame.cpp

SOURCES     += src/metaplots/metaplot.cpp
SOURCES     += src/metaplots/scatter/scatter_metaplot.cpp
SOURCES     += src/metaplots/scatter/scatter_style_frame.cpp
SOURCES     += src/metaplots/scatter/scatter_style.cpp
SOURCES     += src/metaplots/bar/bar_metaplot.cpp
SOURCES     += src/metaplots/bar/bar_style_frame.cpp
SOURCES     += src/metaplots/bar/bar_style.cpp

SOURCES     += src/ihm/forms_widgets/auto_label_widget.cpp
SOURCES     += src/ihm/forms_widgets/linestyle_widget.cpp

SOURCES     += src/metaplots/generic_style_frame.cpp
SOURCES     += src/metaplots/style_mapper.cpp

HEADERS      = src/common/app_context.h
HEADERS     += src/common/singleton.h
HEADERS     += src/common/metagraph.h
HEADERS     += src/ihm/main_window.h
HEADERS     += src/ihm/workspace_frame.h
HEADERS     += src/ihm/graph_container_frame.h
HEADERS     += src/ihm/graph_frame.h
HEADERS     += src/ihm/docks/fs_browser_frame.h
HEADERS     += src/ihm/docks/page_config_frame.h
HEADERS     += src/ihm/docks/graph_config_frame.h
HEADERS     += src/ihm/docks/plot_config_frame.h

HEADERS     += src/metaplots/metaplot.h
HEADERS     += src/metaplots/base_style.h
HEADERS     += src/metaplots/istyle_editor.h
HEADERS     += src/metaplots/scatter/scatter_metaplot.h
HEADERS     += src/metaplots/scatter/scatter_style_frame.h
HEADERS     += src/metaplots/bar/bar_style.h
HEADERS     += src/metaplots/bar/bar_metaplot.h
HEADERS     += src/metaplots/bar/bar_style_frame.h
HEADERS     += src/metaplots/bar/bar_style.h

HEADERS     += src/ihm/forms_widgets/auto_label_widget.h
HEADERS     += src/ihm/forms_widgets/linestyle_widget.h

HEADERS     += src/metaplots/generic_style_frame.h
HEADERS     += src/metaplots/style_field_desc.h
HEADERS     += src/metaplots/style_mapper.h

RESOURCES   += resources/resources.qrc

QT          += core gui widgets webenginewidgets
