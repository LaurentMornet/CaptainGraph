#include "configuration.h"
#include "page.h"
#include <iostream>

void Configuration::set_label(const std::string& label) { 
    _label = label; 
}
short Configuration::new_graph() {
    std::unique_ptr<Graph> ptr = std::make_unique<Graph>();
    short id = ptr->id();
    _graph_stack[id] = std::move(ptr);
    return id;
}
Graph* Configuration::graph(const short& id) {
    for(auto& [key, item]: _graph_stack)
        if(key == id)
            return item.get();
    return nullptr;
}
void Configuration::add_page() {
    _pages.emplace_back(std::make_unique<Page>(this));
}
short Configuration::pages_count() const {
    return _pages.size();
}
Page* Configuration::page(const short& ipage) const {
    if(ipage < 0 || ipage >= pages_count())
        return nullptr;
    return _pages[ipage].get();
}