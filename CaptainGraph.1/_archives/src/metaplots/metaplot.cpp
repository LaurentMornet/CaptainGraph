#include "metaplot.h"

MetaPlot::MetaPlot(std::string prefix) {
    _title = prefix + "_" + std::to_string(MetaPlot::__global__id++);
}

void MetaPlot::set_ancestor(MetaGraph * ancestor) {
    _ancestor = ancestor;
}

MetaGraph * MetaPlot::ancestor() {
    return _ancestor;
}
void MetaPlot::set_title(std::string title) {
    _title = title;
}
std::string & MetaPlot::title() {
    return _title;
}

std::shared_ptr<BaseStyle> & MetaPlot::style() {
    return _style;
}
void MetaPlot::set_style(std::unique_ptr<BaseStyle> & style) {
    _style = std::move(style);
}

void MetaPlot::set_mandatory_data(std::string id, std::string content) {
    _mandatory_data[id] = content;
}

void MetaPlot::apply_style(BaseStyle * base_style) {

}