#include "graph_adapter.h"

GraphAdapter::GraphAdapter(Graph* graph, QObject* parent)
    : QObject(parent), _graph(graph) {}

Graph* GraphAdapter::graph() const { return _graph; }

/* -------------- Interface simple --------------- */

QString GraphAdapter::id() const {
    return QString::fromStdString(_graph->id());
}
QString GraphAdapter::title() const {
    return QString::fromStdString(_graph->title());
}
QString GraphAdapter::x_label() const {
    return QString::fromStdString(_graph->x_label());
}
QString GraphAdapter::y_label() const {
    return QString::fromStdString(_graph->y_label());
}
QString GraphAdapter::z_label() const {
    return QString::fromStdString(_graph->z_label());
}
bool GraphAdapter::grid() const {
    return _graph->grid();
}
bool GraphAdapter::axis_equal() const {
    return _graph->axis_equal();
}
void GraphAdapter::set_title(const QString& title) {
    _graph->set_title(title.toStdString());
    emit title_changed(title);
}
void GraphAdapter::set_x_label(const QString& x_label) {
    _graph->set_x_label(x_label.toStdString());
    emit x_label_changed(x_label);
}
void GraphAdapter::set_y_label(const QString& y_label) {
    _graph->set_y_label(y_label.toStdString());
    emit y_label_changed(y_label);
}
void GraphAdapter::set_z_label(const QString& z_label) {
    _graph->set_z_label(z_label.toStdString());
    emit z_label_changed(z_label);
}
void GraphAdapter::set_grid(bool grid) {
    _graph->set_grid(grid);
    emit grid_changed(grid);
}
void GraphAdapter::set_axis_equal(bool axis_equal) {
    _graph->set_axis_equal(axis_equal);
    emit axis_equal_changed(axis_equal);
}
/* -------------- Gestion des Plots ----------------- */

void GraphAdapter::add_plot() { _graph->add_plot(); }
unsigned int GraphAdapter::plots_count() const { return _graph->plots_count(); }
Plot& GraphAdapter::plot(unsigned int idx) { return _graph->plot(idx); }
QString GraphAdapter::plot_label(unsigned int idx) const { return QString::fromStdString(_graph->plot(idx).label()); }
Plot& GraphAdapter::last_plot() { return _graph->last_plot(); }
void GraphAdapter::swap_plots(int i0, int i1) { _graph->swap_plots(i0, i1); }
void GraphAdapter::delete_plot(int i) { _graph->delete_plot(i); }
/* -------------- Interface Modele ------------------ */

int GraphAdapter::row_count() const {
    return 6;
}

QString GraphAdapter::label_for_row(int row) const {
    static const QString labels[] = {"Title", "X Label", "Y Label", "Z Label", "Grid", "Axis Equal" };
    return (row >= 0 && row < 6) ? labels[row] : QString();
}
QVariant GraphAdapter::value_for_row(int row) const {
    switch(row) {
    case 0: return title();
    case 1: return x_label();
    case 2: return y_label();
    case 3: return z_label();
    case 4: return grid();
    case 5: return axis_equal();
    default: return {};
    }
}

void GraphAdapter::set_value_for_row(int row, const QVariant& value) {
    switch (row) {
    case 0: set_title(value.toString()); break;
    case 1: set_x_label(value.toString()); break;
    case 2: set_y_label(value.toString()); break;
    case 3: set_z_label(value.toString()); break;
    case 4: set_grid(value.toBool()); break;
    case 5: set_axis_equal(value.toBool()); break;
    default: break;
    }
}
bool GraphAdapter::is_editable_row(int row) const { return row >= 0 && row <= 3; }
bool GraphAdapter::is_checkable_row(int row) const { return row >= 4 && row <= 5; }