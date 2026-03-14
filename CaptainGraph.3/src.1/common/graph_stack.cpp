#include "graph_stack.h"

Graph* GraphStack::get(const std::string& id) {
    auto it = _all_graphs.find(id);
    return it != _all_graphs.end() ? it->second.get() : nullptr;
}

void GraphStack::add_graph(std::unique_ptr<Graph> graph) {
    _all_graphs[graph->id()] = std::move(graph);
}

void GraphStack::remove_graph(const std::string& id) {
    _all_graphs.erase(id);
    _orphans_graphs.erase(id);
}

void GraphStack::mark_orphan(const std::string& id) {
    _orphans_graphs.insert(id);
}