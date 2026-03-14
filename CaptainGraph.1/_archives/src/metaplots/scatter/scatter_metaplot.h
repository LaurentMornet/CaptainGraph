#pragma once
#include "../metaplot.h"
#include "scatter_style.h"

class ScatterMetaPlot: public MetaPlot {
public:
    explicit ScatterMetaPlot();
    virtual ~ScatterMetaPlot() = default;
    std::string type_name() const override;
    std::string to_plotly() override;
    std::string to_mpl() override;
};