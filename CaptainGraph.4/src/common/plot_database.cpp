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
    {PlotType::Stem,      "Stem"},
    {PlotType::PColorMesh,"Pseudo Color Mesh"}
};
std::map<PlotType, std::string> Plot::__static__types_icons__ = {
    {PlotType::Scatter,   ":/resources/plottypes/scatter.svg"},
    {PlotType::Line,      ":/resources/plottypes/line.svg"},
    {PlotType::Bar,       ":/resources/plottypes/bar.svg"},
    {PlotType::HLine,     ":/resources/plottypes/hline.svg"},
    {PlotType::VLine,     ":/resources/plottypes/vline.svg"},
    {PlotType::Polar,     ":/resources/plottypes/polar.svg"},
    {PlotType::Scatter3D, ":/resources/plottypes/scatter_3d.svg"},
    {PlotType::Pie,       ":/resources/plottypes/pie.svg"},
    {PlotType::Stem,      ":/resources/plottypes/stem.svg"},
    {PlotType::PColorMesh,":/resources/plottypes/pcolormesh.svg"}
};
std::vector<PlotType> Plot::__static__plots_types_indexes__ = {
    PlotType::Scatter,
    PlotType::Line,
    PlotType::Stem,
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
    {PlotType::Stem,        {"E", "F"}},
    {PlotType::Bar,         {"G", "H"}},
    {PlotType::HLine,       {"Y"}},
    {PlotType::VLine,       {"X"}},
    {PlotType::Polar,       {"D", "A"}},
    {PlotType::Scatter3D,   {"X", "Y", "Z"}},
    {PlotType::Pie,         {"A", "B", "C"}},
    {PlotType::PColorMesh,  {"D", "E", "F"}}
};
std::map<std::string, std::vector<std::string>> Plot::__static__custom_icons__ = {
    {"linestyles", {"none", "solid", "dash", "dot", "dashdot"}},
    {"markershapes", {"none", "circle", "cross", "losange", "plus", "square", "triangle"}}
};
StyleDatabase Plot::__static__style_database__ = {
    {PlotType::Scatter,      {
        StyleItem("show-legend", StyleItemType::Bool, "Show in legend", StyleItemValue(true)),
        StyleItem("line-style", StyleItemType::CustomIcon, "Line Style", StyleItemValue(0), {
            StyleItemValue("linestyles"), StyleItemValue(80), StyleItemValue(20)
        }),
        StyleItem("line-color", StyleItemType::Color,      "Color", StyleItemValue("blue")),
        StyleItem("line-width", StyleItemType::Integer_Range, "Line Width", StyleItemValue(1), {
            StyleItemValue(1), StyleItemValue(10)
        }),
        StyleItem("marker-shape", StyleItemType::CustomIcon, "Marker Shape", StyleItemValue(1), {
            StyleItemValue("markershapes"), StyleItemValue(25), StyleItemValue(25)
        }),
        StyleItem("marker-size", StyleItemType::Integer_Range, "Marker Size", StyleItemValue(1), {
            StyleItemValue(1), StyleItemValue(10)
        }),
        StyleItem("marker-edge-color", StyleItemType::Color, "Marker Edge Color", StyleItemValue("blue")),
        StyleItem("marker-face-color", StyleItemType::Color, "Marker Face Color", StyleItemValue("none"))
    }},
    {PlotType::Line,      {
        StyleItem("line-style", StyleItemType::CustomIcon, "Line Style", StyleItemValue(1), {
            StyleItemValue("linestyles"), StyleItemValue(80), StyleItemValue(20)
        }),
        StyleItem("line-color", StyleItemType::Color,      "Color", StyleItemValue("black")),
        StyleItem("line-width", StyleItemType::Integer_Range, "Line Width", StyleItemValue(1), {
            StyleItemValue(1), StyleItemValue(10)
        }),
        StyleItem("marker-shape", StyleItemType::CustomIcon, "Marker Shape", StyleItemValue(1), {
            StyleItemValue("markershapes"), StyleItemValue(25), StyleItemValue(25)
        }),
        StyleItem("marker-size", StyleItemType::Integer_Range, "Marker Size", StyleItemValue(1), {
            StyleItemValue(1), StyleItemValue(10)
        }),
        StyleItem("marker-edge-color", StyleItemType::Color, "Marker Edge Color", StyleItemValue("blue")),
        StyleItem("marker-face-color", StyleItemType::Color, "Marker Face Color", StyleItemValue("none"))
    }},
    {PlotType::Stem,      {
        StyleItem("line-style", StyleItemType::CustomIcon, "Line Style", StyleItemValue(2), {
            StyleItemValue("linestyles"), StyleItemValue(80), StyleItemValue(20)
        }),
        StyleItem("line-color", StyleItemType::Color,      "Color", StyleItemValue("black")),
        StyleItem("line-width", StyleItemType::Integer_Range, "Line Width", StyleItemValue(1), {
            StyleItemValue(1), StyleItemValue(10)
        }),
        StyleItem("marker-shape", StyleItemType::CustomIcon, "Marker Shape", StyleItemValue(1), {
            StyleItemValue("markershapes"), StyleItemValue(25), StyleItemValue(25)
        }),
        StyleItem("marker-size", StyleItemType::Integer_Range, "Marker Size", StyleItemValue(1), {
            StyleItemValue(1), StyleItemValue(10)
        }),
        StyleItem("marker-edge-color", StyleItemType::Color, "Marker Edge Color", StyleItemValue("blue")),
        StyleItem("marker-face-color", StyleItemType::Color, "Marker Face Color", StyleItemValue("none"))
    }},
    {PlotType::Bar,       {
        StyleItem("face-color", StyleItemType::Color, "Face Color", StyleItemValue("blue")),
        StyleItem("edge-color", StyleItemType::Color, "Face Color", StyleItemValue("black")),
        StyleItem("width", StyleItemType::Double_Range, "Bar Width", StyleItemValue(0.8), {
            StyleItemValue(0.1), StyleItemValue(1.0), StyleItemValue(0.1)
        }),
        StyleItem("alpha", StyleItemType::Double_Range, "Opacity", StyleItemValue(0.8), {
            StyleItemValue(0.1), StyleItemValue(1.0), StyleItemValue(0.1)
        })
    }},
    {PlotType::Polar,     {
        StyleItem("line-style", StyleItemType::CustomIcon, "Line Style", StyleItemValue(0), {
            StyleItemValue("linestyles"), StyleItemValue(80), StyleItemValue(20)
        }),
        StyleItem("line-color", StyleItemType::Color,      "Color", StyleItemValue("blue")),
        StyleItem("line-width", StyleItemType::Integer_Range, "Line Width", 1, {
            StyleItemValue(1), StyleItemValue(10)
        }),
        StyleItem("marker-shape", StyleItemType::CustomIcon, "Marker Shape", StyleItemValue(0), {
            StyleItemValue("markershapes"), StyleItemValue(25), StyleItemValue(25)
        }),
        StyleItem("marker-size", StyleItemType::Integer_Range, "Marker Size", StyleItemValue(1), {
            StyleItemValue(1), StyleItemValue(10)
        })
    }},
    {PlotType::Scatter3D, {
        StyleItem("line-style",         StyleItemType::CustomIcon,      "Line Style", StyleItemValue(1),                { StyleItemValue("linestyles"), StyleItemValue(80), StyleItemValue(20) }),
        StyleItem("line-color",         StyleItemType::Color,           "Color", StyleItemValue("blue")),
        StyleItem("line-width",         StyleItemType::Integer_Range,   "Line Width", StyleItemValue(1),                { StyleItemValue(1), StyleItemValue(10) }),
        StyleItem("marker-shape",       StyleItemType::CustomIcon,      "Marker Shape", StyleItemValue(0),              { StyleItemValue("markershapes"), StyleItemValue(25), StyleItemValue(25) }),
        StyleItem("marker-size",        StyleItemType::Integer_Range,   "Marker Size", StyleItemValue(1),               { StyleItemValue(1), StyleItemValue(10) }),
        StyleItem("marker-edge-color",  StyleItemType::Color,           "Marker Edge Color", StyleItemValue("blue")),
        StyleItem("marker-face-color",  StyleItemType::Color,           "Marker Face Color", StyleItemValue("none"))
    }},
    {PlotType::Pie,  {
        StyleItem("color",              StyleItemType::Color,           "Slice Color", StyleItemValue("blue")),
        StyleItem("edge-color",         StyleItemType::Color,           "Edge Color", StyleItemValue("white")),
        StyleItem("explode",            StyleItemType::Double_Range,    "Explode ratio", StyleItemValue(0.0),          { StyleItemValue(0.0), StyleItemValue(0.5), StyleItemValue(0.01) }),
        StyleItem("shadow",             StyleItemType::Bool,            "Shadow", StyleItemValue(false))
    }},
    {PlotType::HLine,    {
        StyleItem("line-style", StyleItemType::CustomIcon, "Line Style", StyleItemValue(0), {
            StyleItemValue("linestyles"), StyleItemValue(80), StyleItemValue(20)
        }),
        StyleItem("line-color", StyleItemType::Color,      "Color", StyleItemValue("pink")),
        StyleItem("line-width", StyleItemType::Integer_Range, "Line Width", StyleItemValue(1), {
            StyleItemValue(1), StyleItemValue(10)
        })
    }},
    {PlotType::VLine,    {
        StyleItem("line-style", StyleItemType::CustomIcon, "Line Style", StyleItemValue(0), {
            StyleItemValue("linestyles"), StyleItemValue(80), StyleItemValue(20)
        }),
        StyleItem("line-color", StyleItemType::Color,      "Color", StyleItemValue("yellow")),
        StyleItem("line-width", StyleItemType::Integer_Range, "Line Width", 1, {
            StyleItemValue(1), StyleItemValue(10)
        })
    }},
    {PlotType::PColorMesh,  {
        StyleItem("colormap", StyleItemType::String,      "Colormap", StyleItemValue("viridis"))
    }}
};