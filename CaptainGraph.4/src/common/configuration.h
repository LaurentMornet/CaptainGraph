#pragma once

#include <memory>
#include <string>
#include <vector>

#include "graph.h"
#include "page.h"

class Configuration {
private:
    std::string                                _label;
    std::map<short, std::unique_ptr<Graph>>    _graph_stack;
    std::vector<std::unique_ptr<Page>>         _pages;
public:
    void set_label(const std::string& label);
    short new_graph();
    Graph* graph(const short& id);
    void add_page();
    short pages_count() const;
    Page* page(const short& ipage) const;
};
