#include "bar_metaplot.h"
BarMetaPlot::BarMetaPlot(): MetaPlot("Bar") {
    _style = std::make_shared<BarStyle>();
}
std::string BarMetaPlot::type_name() const  { return "bar"; }
std::string BarMetaPlot::to_plotly()        { return "";        }
std::string BarMetaPlot::to_mpl()           { return "";        }