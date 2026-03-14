#pragma once

#include <memory>
#include <set>
#include <string>
#include <unordered_map>

#include "graph.h"

class GraphStack {
private:
    std::unordered_map<std::string, std::unique_ptr<Graph>> _all_graphs;
    std::set<std::string> _orphans_graphs;
protected:
public:
    Graph* get(const std::string& id);
    void add_graph(std::unique_ptr<Graph> graph);
    void remove_graph(const std::string& id);
    void mark_orphan(const std::string& id);
};