#include "plot.h"

std::map<PlotType, std::string> Plot::__static__plots_types_map__ = {
    {PlotType::Scatter,   "Scatter"},
    {PlotType::Line,      "Line"},
    {PlotType::Bar,       "Bar"},
    {PlotType::HLine,     "Horizontal line"},
    {PlotType::VLine,     "Vertical line"},
    {PlotType::Polar,     "Polar"},
    {PlotType::Scatter3D, "Scatter 3D"},
    {PlotType::Pie,       "Pie"},
    {PlotType::PColorMesh,"Pseudo Color Mesh"}
};
std::vector<PlotType> Plot::__static__plots_types_indexes__ = {
    PlotType::Scatter,
    PlotType::Line,
    PlotType::Bar,
    PlotType::HLine,
    PlotType::VLine,
    PlotType::Polar,
    PlotType::Scatter3D,
    PlotType::Pie,
    PlotType::PColorMesh
};
std::map<PlotType, std::vector<std::string>> Plot::__static__features__ = {
    {PlotType::Scatter,     {"X", "Y"}},
    {PlotType::Line,        {"C", "D"}},
    {PlotType::Bar,         {"X", "Y"}},
    {PlotType::HLine,       {"Y"}},
    {PlotType::VLine,       {"X"}},
    {PlotType::Polar,       {"D", "A"}},
    {PlotType::Scatter3D,   {"X", "Y", "Z"}},
    {PlotType::Pie,         {"A", "B", "C"}},
    {PlotType::PColorMesh,  {"D", "E", "F"}}
};
StyleDatabase Plot::__static__style_database__ = {
    {PlotType::Scatter,      {
        StyleItem("line-style", StyleItemType::CustomIcon, "Line Style", 0, {
            StyleItemValue("line_style"), StyleItemValue(80), StyleItemValue(20)
        }),
        StyleItem("line-color", StyleItemType::Color,      "Color", "blue"),
        StyleItem("line-width", StyleItemType::Integer_Range, "Line Width", 1, {
            StyleItemValue(1), StyleItemValue(10)
        }),
        StyleItem("marker-shape", StyleItemType::CustomIcon, "Marker Shape", 0, {
            StyleItemValue("marker_shape"), StyleItemValue(25), StyleItemValue(25)
        }),
        StyleItem("marker-size", StyleItemType::Integer_Range, "Marker Size", 1, {
            StyleItemValue(1), StyleItemValue(10)
        }),
        StyleItem("marker-edge-color", StyleItemType::Color, "Marker Edge Color", "blue"),
        StyleItem("marker-face-color", StyleItemType::Color, "Marker Face Color", "none")
    }},
    {PlotType::Line,      {
        StyleItem("line-style", StyleItemType::CustomIcon, "Line Style", 1, {
            StyleItemValue("line_style"), StyleItemValue(80), StyleItemValue(20)
        }),
        StyleItem("line-color", StyleItemType::Color,      "Color", "black"),
        StyleItem("line-width", StyleItemType::Integer_Range, "Line Width", 1, {
            StyleItemValue(1), StyleItemValue(10)
        }),
        StyleItem("marker-shape", StyleItemType::CustomIcon, "Marker Shape", 1, {
            StyleItemValue("marker_shape"), StyleItemValue(25), StyleItemValue(25)
        }),
        StyleItem("marker-size", StyleItemType::Integer_Range, "Marker Size", 1, {
            StyleItemValue(1), StyleItemValue(10)
        }),
        StyleItem("marker-edge-color", StyleItemType::Color, "Marker Edge Color", "blue"),
        StyleItem("marker-face-color", StyleItemType::Color, "Marker Face Color", "none")
    }},
    {PlotType::Bar,       {
        StyleItem("face-color", StyleItemType::Color, "Face Color", "blue"),
        StyleItem("edge-color", StyleItemType::Color, "Face Color", "black"),
        StyleItem("width", StyleItemType::Double_Range, "Bar Width", 0.8, {
            StyleItemValue(0.1), StyleItemValue(1.0), StyleItemValue(0.1)
        }),
        StyleItem("alpha", StyleItemType::Double_Range, "Opacity", 0.8, {
            StyleItemValue(0.1), StyleItemValue(1.0), StyleItemValue(0.1)
        })
    }},
    {PlotType::Polar,     {
        StyleItem("line-style", StyleItemType::CustomIcon, "Line Style", 0, {
            StyleItemValue("line_style"), StyleItemValue(80), StyleItemValue(20)
        }),
        StyleItem("line-color", StyleItemType::Color,      "Color", "blue"),
        StyleItem("line-width", StyleItemType::Integer_Range, "Line Width", 1, {
            StyleItemValue(1), StyleItemValue(10)
        }),
        StyleItem("marker-shape", StyleItemType::CustomIcon, "Marker Shape", 0, {
            StyleItemValue("marker_shape"), StyleItemValue(25), StyleItemValue(25)
        }),
        StyleItem("marker-size", StyleItemType::Integer_Range, "Marker Size", 1, {
            StyleItemValue(1), StyleItemValue(10)
        })
    }},
    {PlotType::Scatter3D, {
        StyleItem("line-style",         StyleItemType::CustomIcon,      "Line Style", 0,                { StyleItemValue("line_style"), StyleItemValue(80), StyleItemValue(20) }),
        StyleItem("line-color",         StyleItemType::Color,           "Color", "blue"),
        StyleItem("line-width",         StyleItemType::Integer_Range,   "Line Width", 1,                { StyleItemValue(1), StyleItemValue(10) }),
        StyleItem("marker-shape",       StyleItemType::CustomIcon,      "Marker Shape", 0,              { StyleItemValue("marker_shape"), StyleItemValue(25), StyleItemValue(25) }),
        StyleItem("marker-size",        StyleItemType::Integer_Range,   "Marker Size", 1,               { StyleItemValue(1), StyleItemValue(10) }),
        StyleItem("marker-edge-color",  StyleItemType::Color,           "Marker Edge Color", "blue"),
        StyleItem("marker-face-color",  StyleItemType::Color,           "Marker Face Color", "none")
    }},
    {PlotType::Pie,  {
        StyleItem("color",              StyleItemType::Color,           "Slice Color", "blue"),
        StyleItem("edge-color",         StyleItemType::Color,           "Edge Color", "white"),
        StyleItem("explode",            StyleItemType::Double_Range,    "Explode ratio", 0.0,          { StyleItemValue(0.0), StyleItemValue(0.5), StyleItemValue(0.05) }),
        StyleItem("shadow",             StyleItemType::Bool,            "Shadow", false)
    }},
    {PlotType::HLine,    {
        StyleItem("line-style", StyleItemType::CustomIcon, "Line Style", 0, {
            StyleItemValue("line_style"), StyleItemValue(80), StyleItemValue(20)
        }),
        StyleItem("line-color", StyleItemType::Color,      "Color", "pink"),
        StyleItem("line-width", StyleItemType::Integer_Range, "Line Width", 1, {
            StyleItemValue(1), StyleItemValue(10)
        })
    }},
    {PlotType::VLine,    {
        StyleItem("line-style", StyleItemType::CustomIcon, "Line Style", 0, {
            StyleItemValue("line_style"), StyleItemValue(80), StyleItemValue(20)
        }),
        StyleItem("line-color", StyleItemType::Color,      "Color", "yellow"),
        StyleItem("line-width", StyleItemType::Integer_Range, "Line Width", 1, {
            StyleItemValue(1), StyleItemValue(10)
        })
    }},
    {PlotType::PColorMesh,  {
        StyleItem("colormap", StyleItemType::String,      "Colormap", "viridis")
    }}
};