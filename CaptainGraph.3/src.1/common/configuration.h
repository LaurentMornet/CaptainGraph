#pragma once

#include <memory>
#include <string>
#include <vector>

#include "graph.h"
#include "graph_stack.h"
#include "layout.h"

class Layout;

class Configuration {
private:
    std::string                                     _label;
    GraphStack                                      _graph_stack;
    std::vector<std::string>                        _layouts_titles;
    std::vector<std::unique_ptr<Layout>>            _layouts;
    std::vector<std::map<std::string, std::string>> _graph_map;
public:
    void set_label(const std::string& label);
    Graph* get_graph(const std::string& id);                      // retour d'un graphe par son id
    int get_layout_index(Layout* layout);
    Graph* get_layout_graph(unsigned int, const std::string& id); // retour d'un graphe par sa position dans le layout global
    GraphStack& graph_stack();
    Layout& add_layout();
    unsigned int layouts_count() const;
    Layout& layout(unsigned int idx);
    void set_layout_title(unsigned int idx, const std::string& title);
    std::string layout_title(unsigned int idx) const;
    void print() const;
    // helper
    Layout& sublayout(unsigned int idx, const std::string& id);
};
