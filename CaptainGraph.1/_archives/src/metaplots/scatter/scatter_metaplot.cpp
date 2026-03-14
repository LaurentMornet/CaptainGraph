#include "scatter_metaplot.h"
ScatterMetaPlot::ScatterMetaPlot(): MetaPlot("Scatter") {
    _style = std::make_shared<ScatterStyle>();
}
std::string ScatterMetaPlot::type_name() const  { return "scatter"; }
std::string ScatterMetaPlot::to_plotly()        { return "";        }
std::string ScatterMetaPlot::to_mpl()           { return "";        }