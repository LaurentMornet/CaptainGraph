#pragma once
#include "../metaplot.h"
#include "bar_style.h"

class BarMetaPlot: public MetaPlot {
public:
    explicit BarMetaPlot();
    virtual ~BarMetaPlot() = default;
    std::string type_name() const override;
    std::string to_plotly() override;
    std::string to_mpl() override;
};