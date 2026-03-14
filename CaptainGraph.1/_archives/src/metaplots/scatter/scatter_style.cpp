#include "scatter_style.h"
ScatterStyle::ScatterStyle(): line_style("solid"), line_color("blue"), marker("diamond") {}
std::unique_ptr<BaseStyle> ScatterStyle::clone() const {
    return std::make_unique<ScatterStyle>(*this);
}