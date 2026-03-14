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

// #include "custom_widgets/auto_lineedit.h"
#include "../adapter/graph_adapter.h"
#include "custom_widgets/chips_container.h"

class GraphFrame;
class GraphDataModel;
class GraphDataDelegate;

/**
 * @brief Widget principal d'affichage et de modification des propriétés d'un graphe.
 */
class GraphFrame : public QFrame {
    Q_OBJECT
private:
    static GraphFrame* __instance; ///< Instance unique
    explicit GraphFrame(QWidget* parent = nullptr);

    QPointer<GraphAdapter> _graph_adapter;
    QFrame* _main_frame;
    QTableView* _data_table;
    GraphDataModel* _data_model;
    ChipsContainer* _plots_container;
private slots:
    void _cb_new_plot();
public:
    ~GraphFrame() override;

    GraphFrame(const GraphFrame&) = delete;
    GraphFrame& operator=(const GraphFrame&) = delete;

    static GraphFrame& get_instance(QWidget* parent = nullptr);

    void set_graph_adapter(GraphAdapter* graph_adapter);
    QSize sizeHint() const override;
};

/**
 * @brief Modèle de données associé à la table d'édition des propriétés du graphe.
 */
class GraphDataModel : public QStandardItemModel {
    Q_OBJECT
private:
    QPointer<GraphAdapter> _adapter;
public:
    GraphAdapter* adapter();
    explicit GraphDataModel(GraphAdapter* frame);
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    void set_adapter(GraphAdapter* adapter);
    void update_model();
};

/**
 * @brief Delegate de gestion des éditeurs de cellules dans la table de configuration du graphe.
 */
class GraphDataDelegate : public QStyledItemDelegate {
    Q_OBJECT
private:
    QPointer<GraphDataModel> _model;
public:
    explicit GraphDataDelegate(GraphDataModel* frame);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                              const QModelIndex& index) const override;
};
