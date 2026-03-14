#include "graph.h"

#include <sstream>

int Graph::__static__id_count__ = 0;

Graph::Graph() {
    Graph::__static__id_count__ ++;
    std::ostringstream sid;
    sid << "graph_" << Graph::__static__id_count__;
    set_id(sid.str());
    set_title(id());
}

/**
 * Accesseurs / Modificateurs
 */
std::string Graph::id() const { return _id; }
void Graph::set_id(const std::string id) { _id = id; }
std::string Graph::title() const { return _title; }
void Graph::set_title(const std::string title) { _title = title; }
std::string Graph::x_label() const { return _x_label; }
void Graph::set_x_label(const std::string& label) { _x_label = label; }
std::string Graph::y_label() const { return _y_label; }
void Graph::set_y_label(const std::string& label) { _y_label = label; }
std::string Graph::z_label() const { return _z_label; }
void Graph::set_z_label(const std::string& label) { _z_label = label; }
bool Graph::grid() const { return _grid; }
void Graph::set_grid(bool grid) { _grid = grid; }
bool Graph::axis_equal() const { return _axis_equal; }
void Graph::set_axis_equal(bool axis_equal) { _axis_equal = axis_equal; }
/**
 * Gestion des plots
 */
Plot* Graph::add_plot() {
    _plots.emplace_back(Plot());
    return &_plots.back();
}
unsigned int Graph::plots_count() {
    return _plots.size();
}
Plot& Graph::plot(unsigned int idx) {
    return _plots[idx];
}
Plot& Graph::last_plot() {
    return _plots[_plots.size() - 1];
}
void Graph::swap_plots(int i0, int i1) {
    std::swap(_plots[i0], _plots[i1]);
}
void Graph::delete_plot(int i) {
    _plots.erase(_plots.begin() + i);
}