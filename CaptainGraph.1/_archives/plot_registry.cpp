#include "plot.h"

std::vector<std::string> Plot::PlotTypeNames = {};

std::map<std::string, std::vector<std::string>> Plot::CustomIconTypes = {
    {"plot_type", {":/resources/plottypes/plot_2d", ":/resources/plottypes/scatter_2d", ":/resources/plottypes/bar_2d", ":/resources/plottypes/plot_3d"}},
    {"line_style", {":/resources/linestyles/none", ":/resources/linestyles/solid", ":/resources/linestyles/dot", ":/resources/linestyles/dash", ":/resources/linestyles/dashdot"}},
    {"marker_shape", {":/resources/markershapes/none", ":/resources/markershapes/circle", ":/resources/markershapes/square", ":/resources/markershapes/losange"}}
};

std::map<std::string, Plot> Plot::base_plots_registry = {
    {"plot_2d",      Plot("plot_2d")},
    {"scatter_2d",   Plot("scatter_2d")},
    {"bar_2d",       Plot("bar_2d")},
    {"plot_3d",      Plot("plot_3d")},
};

void Plot::load_base_plots_registry() {

    Plot::PlotTypeNames.push_back("plot_2d");
    Plot::base_plots_registry["plot_2d"]._style_keys = {
        "label_test",
        "int_test",
        "double_test",
        "double_rng_test",
        "line_style",        
        "line_color",        
        "line_width",        
        "marker",            
        "marker_size",       
        "marker_face_color", 
        "marker_edge_color",
        "axis_equal"
    };
    Plot::base_plots_registry["plot_2d"]._style = {  
        {"label_test",          {StyleItemType::STRING,      StyleItemMode::BASIC,       "Test Text",            {},                                     std::string("Hello")}},
        {"int_test",            {StyleItemType::INTEGER,     StyleItemMode::BASIC,       "Test Integer",         {},                                     42}},
        {"double_test",         {StyleItemType::DOUBLE,      StyleItemMode::BASIC,       "Test Double",          {},                                     -9999.999}},
        {"double_rng_test",     {StyleItemType::DOUBLE_RANGE,StyleItemMode::BASIC,       "Test Double range",    {  StyleItemArg(-1.5), 
                                                                                                                    StyleItemArg(1.6), 
                                                                                                                    StyleItemArg(0.1)},                  0.0}},
        {"line_style",          {StyleItemType::CUSTOM_ICON, StyleItemMode::BASIC,       "Line style",           {  StyleItemArg(std::string("line_style")), 
                                                                                                                    StyleItemArg(80), 
                                                                                                                    StyleItemArg(20)},                   3}},
        {"line_color",          {StyleItemType::COLOR,       StyleItemMode::BASIC,       "Line color",           {},                                     std::string("red")}},
        {"line_width",          {StyleItemType::INT_RANGE,   StyleItemMode::BASIC,       "Line width",           {  StyleItemArg(1), 
                                                                                                                    StyleItemArg(10)},                   1}},
        {"marker",              {StyleItemType::CUSTOM_ICON, StyleItemMode::BASIC,       "Marker",               {  StyleItemArg(std::string("marker_shape")), 
                                                                                                                    StyleItemArg(25), 
                                                                                                                    StyleItemArg(25)},                   1}},
        {"marker_size",         {StyleItemType::INT_RANGE,   StyleItemMode::ADVANCED,    "Marker size",          {  StyleItemArg(1), 
                                                                                                                    StyleItemArg(10)},                   1}},
        {"marker_face_color",   {StyleItemType::COLOR,       StyleItemMode::BASIC,       "Marker face color",    {},                                     std::string("white")}}, 
        {"marker_edge_color",   {StyleItemType::COLOR,       StyleItemMode::BASIC,       "Marker edge color",    {},                                     std::string("red")}},
        {"axis_equal",          {StyleItemType::BOOL,        StyleItemMode::ADVANCED,    "Axis Equal",           {},                                     true}}
    };
    Plot::base_plots_registry["plot_2d"]._features_names = {"X", "Y"};
    Plot::base_plots_registry["plot_2d"]._features_values = {"", ""};
    Plot::PlotTypeNames.push_back("scatter_2d");
    Plot::base_plots_registry["scatter_2d"]._style_keys = {
        "bar_face_color",
        "bar_edge_color"
    };
    Plot::base_plots_registry["scatter_2d"]._style = {  
        {"bar_face_color",      {StyleItemType::COLOR,       StyleItemMode::BASIC,       "Bar face color",       {},                                     std::string("blue")}},
        {"bar_edge_color",      {StyleItemType::COLOR,       StyleItemMode::ADVANCED,    "Bar edge color",       {},                                     std::string("blue")}}
    };
    Plot::base_plots_registry["scatter_2d"]._features_names = {"A", "B"};
    Plot::base_plots_registry["scatter_2d"]._features_values = {"", ""};
    Plot::PlotTypeNames.push_back("bar_2d");
    Plot::base_plots_registry["bar_2d"]._style_keys = {
        "bar_face_color",
        "bar_edge_color"
    };
    Plot::base_plots_registry["bar_2d"]._style = {  
        {"bar_face_color",      {StyleItemType::COLOR,       StyleItemMode::BASIC,       "Bar face color",       {},                                     std::string("blue")}},
        {"bar_edge_color",      {StyleItemType::COLOR,       StyleItemMode::ADVANCED,    "Bar edge color",       {},                                     std::string("blue")}}
    };
    Plot::base_plots_registry["bar_2d"]._features_names = {"C", "D", "E", "F"};
    Plot::base_plots_registry["bar_2d"]._features_values = {"", ""};
    Plot::PlotTypeNames.push_back("plot_3d");
    Plot::base_plots_registry["plot_3d"]._style_keys = {
        "line_style",        
        "line_color",        
        "line_width",        
        "marker",            
        "marker_size",       
        "marker_face_color", 
        "marker_edge_color",
        "axis_equal"
    };
    Plot::base_plots_registry["plot_3d"]._style = {  
        {"line_style",          {StyleItemType::CUSTOM_ICON, StyleItemMode::BASIC,       "Line style",           {  StyleItemArg(std::string("line_style")), 
                                                                                                                    StyleItemArg(80), 
                                                                                                                    StyleItemArg(20)},                   2}},
        {"line_color",          {StyleItemType::COLOR,       StyleItemMode::BASIC,       "Line color",           {},                                     std::string("red")}},
        {"line_width",          {StyleItemType::INT_RANGE,   StyleItemMode::BASIC,       "Line width",           {  StyleItemArg(1), 
                                                                                                                    StyleItemArg(10)},                   1}},
        {"marker",              {StyleItemType::CUSTOM_ICON, StyleItemMode::BASIC,       "Marker",               {  StyleItemArg(std::string("marker_shape")), 
                                                                                                                    StyleItemArg(25), 
                                                                                                                    StyleItemArg(25)},                   2}},
        {"marker_size",         {StyleItemType::INT_RANGE,   StyleItemMode::ADVANCED,    "Marker size",          {  StyleItemArg(1), 
                                                                                                                    StyleItemArg(10)},                   1}},
        {"marker_face_color",   {StyleItemType::COLOR,       StyleItemMode::BASIC,       "Marker face color",    {},                                     std::string("white")}}, 
        {"marker_edge_color",   {StyleItemType::COLOR,       StyleItemMode::BASIC,       "Marker edge color",    {},                                     std::string("red")}},
        {"axis_equal",          {StyleItemType::BOOL,        StyleItemMode::ADVANCED,    "Axis Equal",           {},                                     true}}
    };
    Plot::base_plots_registry["plot_3d"]._features_names = {"G", "H", "I", "J"};
    Plot::base_plots_registry["plot_3d"]._features_values = {"", "", ""};
}