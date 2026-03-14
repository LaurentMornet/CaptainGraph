#pragma once

#include <vector>
#include <string>
#include <map>

#include <QAbstractListModel>
#include <QFrame>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QToolBar>
#include <QLabel>
#include <QListView>
#include <QCheckBox>


#include "../common/graph.h"
#include "plot_config_frame.h"

class GraphConfigFrame;

class PlotListModel : public QAbstractListModel {
private:
    GraphConfigFrame* _frame;
public:
    explicit PlotListModel(GraphConfigFrame * frame);
    virtual ~PlotListModel() = default;
    void set_list_data();
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role) const override;
};

class GraphConfigFrame : public QFrame {
    // Q_OBJECT
    friend class PlotListModel;
private:
    Graph* _ref_graph;
    QFrame* _box_header;
    QFrame* _box_params;
    QToolBar* _toolbar;
    QLineEdit* _edt_title;
    PlotListModel _list_plots_model;
    QListView* _list_plots;
private slots:
    void _cb_validate();
    void _cb_reset();
    void _cb_title_changed();
    void _cb_axis_equal_checked(bool checked);
    void _cb_grid_checked(bool checked);
    void _cb_new_plot();
    void _cb_delete_plot();
    void _cb_list_pressed(const QModelIndex & index);
    void _cb_up();
    void _cb_down();
private:
    explicit GraphConfigFrame(QWidget* parent = nullptr);
    virtual ~GraphConfigFrame() = default;
    QSize sizeHint() const override;
public:
    static GraphConfigFrame * instance;
    static GraphConfigFrame * get_instance(QWidget * parent = nullptr);
    void set_graph(Graph* graph);
    void set_plot(Plot* plot);
    QString help() { return QString("foo"); }
};
