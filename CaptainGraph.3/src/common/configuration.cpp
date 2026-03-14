#include "configuration.h"
#include <iostream>

void Configuration::set_label(const std::string& label) { _label = label; }

GraphStack& Configuration::graph_stack() {
    return _graph_stack;
}

Graph* Configuration::get_graph(const std::string& id) {
    return _graph_stack.get(id);
}
int Configuration::get_layout_index(Layout* layout) {
    int idx=0;
    for(unsigned int i=0; i<_layouts.size(); i++)
        if(_layouts[idx].get() == layout) { idx=i; return idx; };
    return -1;
}
Graph* Configuration::get_layout_graph(unsigned int layout_idx, const std::string& id) {
    if(this->layout(layout_idx).get(id)) {
        if(_graph_map[layout_idx].count(id) == 0) {
            auto g = std::make_unique<Graph>();
            _graph_map[layout_idx][id] = g->id();
            graph_stack().add_graph(std::move(g));
        }
        return get_graph(_graph_map[layout_idx][id]);
    }
    return nullptr;
}
Layout& Configuration::add_layout() {
    _layouts.emplace_back(std::make_unique<Layout>(this, nullptr, LayoutType::Horizontal));
    _layouts_titles.emplace_back("");
    _graph_map.emplace_back(std::map<std::string, std::string>({}));
    return layout(_layouts.size() - 1);
}
Layout& Configuration::layout(unsigned int idx) {
    return *_layouts[idx];
}
void Configuration::print() const {
    std::cout << "=== Configuration: " << _label << " ===" << std::endl;
    for(unsigned int idx = 0; idx < _layouts.size(); idx++) {
        std::cout << "Layout: " << _layouts_titles[idx] << std::endl;
        _layouts[idx]->dump(3);
    }
}
void Configuration::set_layout_title(unsigned int idx, const std::string& title) {
    _layouts_titles[idx] = title;
}
std::string Configuration::layout_title(unsigned int idx) const {
    return _layouts_titles[idx];
}
// Helper
Layout& Configuration::sublayout(unsigned int idx, const std::string& id) {
    return layout(idx).get(id)->get();
}