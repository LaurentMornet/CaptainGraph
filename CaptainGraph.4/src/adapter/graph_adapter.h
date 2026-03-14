#pragma once
#include <QObject>
#include <QVariant>
#include "../common/graph.h"

class GraphAdapter: public QObject {
    Q_OBJECT
private:
    Graph* _graph;
public:
    explicit GraphAdapter(Graph* graph, QObject* parent = nullptr);
    Graph* graph() const;
    // interface simple
    short id() const;
    QString title() const;
    QString x_label() const;
    QString y_label() const;
    QString z_label() const;
    bool grid() const;
    bool axis_equal() const;
    void set_title(const QString& title);
    void set_x_label(const QString& x_label);
    void set_y_label(const QString& y_label);
    void set_z_label(const QString& z_label);
    void set_grid(bool grid);
    void set_axis_equal(bool axis_equal);
    // gestion des plots
    void add_plot();
    short plots_count() const;
    Plot& plot(unsigned int idx);
    PlotType plot_type(unsigned int idx) const;
    QString plot_label(unsigned int idx) const;
    Plot& last_plot();
    // interface vers modele
    QString label_for_row(int row) const;
    QVariant value_for_row(int row) const;
    void set_value_for_row(int row, const QVariant& value);
    bool is_editable_row(int row) const;
    bool is_checkable_row(int row) const;
    int row_count() const;
    void swap_plots(int i0, int i1);
    void delete_plot(int i);
signals:
    void title_changed(QString);
    void x_label_changed(QString);
    void y_label_changed(QString);
    void z_label_changed(QString);
    void grid_changed(bool);
    void axis_equal_changed(bool);
};