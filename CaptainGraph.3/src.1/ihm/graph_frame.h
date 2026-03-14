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
#include <QToolBar>
#include <QMap>

class MainWindow;
#include "main_window.h"
#include "../adapter/graph_adapter.h"

/**
 * @brief Modèle Qt représentant la liste des courbes (plots) d’un graphe.
 */
class PlotListModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit PlotListModel(QObject* parent = nullptr);
    ~PlotListModel() override = default;

    void set_adapter(GraphAdapter* adapter);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    /// Force un rafraîchissement complet de la liste
    void update_model();

private:
    GraphAdapter* _adapter = nullptr;
};


/**
 * @brief Widget principal pour la configuration et l’édition d’un graphe.
 *
 * Permet d’éditer :
 *  - Titre et labels d’axes
 *  - Options de grille et d’axes
 *  - Gestion de la liste de plots (ajout / suppression / ordre)
 */
class GraphFrame : public QFrame {
    Q_OBJECT
public:
    explicit GraphFrame(MainWindow* parent);
    virtual ~GraphFrame() override = default;

    GraphFrame(const GraphFrame&) = delete;
    GraphFrame& operator=(const GraphFrame&) = delete;

    void set_graph_adapter(GraphAdapter* graph_adapter);
    void update_from_values();

    QSize sizeHint() const override;

private slots:
    void _cb_new_plot();
    void _cb_plot_up();
    void _cb_plot_down();
    void _cb_delete_plot();
    void _cb_plot_list_selection_changed();
private:
    MainWindow* _main_window = nullptr;
    QPointer<GraphAdapter> _graph_adapter;

    // Composants UI
    QFrame* _main_frame = nullptr;
    QLineEdit* _edt_title = nullptr;
    QLineEdit* _edt_xlabel = nullptr;
    QLineEdit* _edt_ylabel = nullptr;
    QLineEdit* _edt_zlabel = nullptr;
    QCheckBox* _chk_grid = nullptr;
    QCheckBox* _chk_axis = nullptr;

    PlotListModel* _plots_list_model = nullptr;
    QListView* _plots_list = nullptr;

    // Sauvegarde du dernier plot sélectionné pour chaque graph
    QMap<QString, int> _last_selected_plot;

    void _set_plotframe_plot(const short& idx);
};
