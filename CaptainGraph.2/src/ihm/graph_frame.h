#pragma once

#include <QCheckBox>
#include <QFormLayout>
#include <QFrame>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QTableView>
#include <QHeaderView>
#include <QPointer>
#include <QListView>
#include <QAbstractListModel>

class MainWindow;
#include "main_window.h"
#include "../adapter/graph_adapter.h"

class GraphFrame;

class PlotListModel: public QAbstractListModel {
    Q_OBJECT
private:
    GraphAdapter* _adapter;
public:
    explicit PlotListModel(QObject* parent);
    virtual ~PlotListModel() = default;
    void set_adapter(GraphAdapter* adapter);
    int rowCount(const QModelIndex& parent=QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    void update_model();
};

/**
 * @brief Widget principal d'affichage et de modification des propriétés d'un graphe.
 */
class GraphFrame : public QFrame {
    Q_OBJECT
public:
    explicit GraphFrame(MainWindow* parent);
private:
    MainWindow* _main_window;
    QPointer<GraphAdapter> _graph_adapter;
    QFrame* _main_frame;    
    QLineEdit* _edt_title;
    QLineEdit* _edt_xlabel;
    QLineEdit* _edt_ylabel;
    QLineEdit* _edt_zlabel;
    QCheckBox* _chk_grid;
    QCheckBox* _chk_axis;
    PlotListModel* _plots_list_model;
    QListView* _plots_list;
    QMap<QString, int> _last_selected_plot;

private slots:
    void _cb_new_plot();
    void _cb_plot_up();
    void _cb_plot_down();
    void _cb_delete_plot();
    void _cb_plot_list_selection_changed();
public:
    ~GraphFrame() override;
    GraphFrame(const GraphFrame&) = delete;
    GraphFrame& operator=(const GraphFrame&) = delete;
    void set_graph_adapter(GraphAdapter* graph_adapter);
    void update_from_values();
    QSize sizeHint() const override;
};