#include "graph.h"
#include <sstream>

/* ========================================================================== */
/*                              Constructeur                                  */
/* ========================================================================== */
int Graph::GRAPH_NUMBER = 0;

Graph::Graph()
    : _axis_equal(false) {
    Graph::GRAPH_NUMBER ++;
    std::ostringstream stitle;
    stitle << "Graph " << Graph::GRAPH_NUMBER;
    set_title(stitle.str());
    _labels = {"X", "Y"};
}

void Graph::from(const Graph& other) {
    if (this == &other)
        return;
    _title = other._title;
    _plots = other._plots; // copie profonde des plots (via opérateur = de Plot)
}

/* ========================================================================== */
/*                            Gestion des plots                               */
/* ========================================================================== */

Plot * Graph::add_plot(Plot* plot) {
    if(!plot)
        _plots.emplace_back(Plot(this));
    else
        _plots.push_back(*plot);
    return &_plots.back();
}

void Graph::remove_last_plot() {
    _plots.pop_back();
}

void Graph::remove_plot(Plot* plot) {
    int idx = index_of_plot(plot);
    if(idx >= 0)
        _plots.erase(_plots.begin() + idx);
}

unsigned short Graph::plots_count() const noexcept {
    return static_cast<unsigned short>(_plots.size());
}

Plot& Graph::plot(unsigned short index) {
    if (index >= _plots.size()) {
        throw std::out_of_range("Graph::plot(): index hors limites");
    }
    return _plots[index];
}

int Graph::index_of_plot(Plot * plot) {
    for(int i=0; i<(int)_plots.size(); i++)
        if(plot == &_plots[i]) return i;
    return -1;
}

/* ========================================================================== */
/*                            Titres et labels                                */
/* ========================================================================== */

void Graph::set_title(const std::string& title) {
    _title = title;
}

std::string Graph::title() const noexcept {
    return _title;
}

unsigned short Graph::label_count() const noexcept {
    return static_cast<unsigned short>(_labels.size());
}

unsigned short Graph::max_labels() const noexcept {
    unsigned short max_feat = 0;
    for (const auto& p : _plots) {
        max_feat = std::max<unsigned short>(
            max_feat,
            static_cast<unsigned short>(p.features_count())
        );
    }
    return max_feat;
}

void Graph::set_label(unsigned short index, const std::string& label) {
    if (index >= _labels.size()) {
        throw std::out_of_range("Graph::set_label(): index hors limites");
    }
    _labels[index] = label;
}

std::string Graph::label(unsigned short index) const {
    if (index >= _labels.size()) {
        throw std::out_of_range("Graph::label(): index hors limites");
    }
    return _labels[index];
}

void Graph::resize_labels(unsigned short count) {
    _labels.resize(count, "");
}

/* ========================================================================== */
/*                              Options diverses                              */
/* ========================================================================== */

void Graph::set_axis_equal(bool equal) noexcept {
    _axis_equal = equal;
}

bool Graph::axis_equal() const noexcept {
    return _axis_equal;
}
