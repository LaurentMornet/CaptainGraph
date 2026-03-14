#include "graph_frame.h"
#include "ihm_tools.h"
#include <QDebug>
#include <QScrollArea>
#include <QToolBar>

/* -------------------------------------------------------------------------- */
/*                            PlotListModel class                             */
/* -------------------------------------------------------------------------- */
PlotListModel::PlotListModel(QObject* parent)
    : QAbstractListModel(parent) {}
void PlotListModel::set_adapter(GraphAdapter* adapter) {
    if (_adapter == adapter)
        return;
    _adapter = adapter;
    update_model();
}
int PlotListModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return _adapter ? _adapter->plots_count() : 0;
}
QVariant PlotListModel::data(const QModelIndex& index, int role) const {
    if (!_adapter || !index.isValid())
        return {};

    switch (role) {
        case Qt::DisplayRole:
            return _adapter->plot_label(index.row());
        case Qt::DecorationRole:
            return IhmTools::load_colored_icon(
                QString::fromStdString(Plot::__static__types_icons__[
                    _adapter->plot_type(index.row())]));
        default:
            return {};
    }
}
void PlotListModel::update_model() {
    beginResetModel();
    endResetModel();
}
/* -------------------------------------------------------------------------- */
/*                               GraphFrame class                             */
/* -------------------------------------------------------------------------- */

GraphFrame::GraphFrame(MainWindow* parent)
    : QFrame(parent), _main_window(parent) {

    // Layout principal
    auto* main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(main_layout);

    /* --- Conteneur principal caché tant qu’aucun graphe n’est assigné --- */
    _main_frame = new QFrame(this);
    main_layout->addWidget(_main_frame);
    auto* layout = new QVBoxLayout(_main_frame);

    /* ------------------------- Propriétés du graphe ------------------------- */
    auto* header_frame = new QFrame(this);
    auto* header_layout = new QFormLayout(header_frame);
    header_layout->setContentsMargins(0, 0, 0, 0);
    header_layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    header_frame->setLayout(header_layout);

    _edt_title  = new QLineEdit(header_frame);
    _edt_xlabel = new QLineEdit(header_frame);
    _edt_ylabel = new QLineEdit(header_frame);
    _edt_zlabel = new QLineEdit(header_frame);
    header_layout->addRow("Title",  _edt_title);
    header_layout->addRow("X Label", _edt_xlabel);
    header_layout->addRow("Y Label", _edt_ylabel);
    header_layout->addRow("Z Label", _edt_zlabel);

    // Options "Grid" et "Axis Equal"
    auto* subframe = new QFrame(header_frame);
    auto* sublayout = new QHBoxLayout(subframe);
    sublayout->setContentsMargins(0, 0, 0, 0);
    _chk_grid = new QCheckBox("Grid", header_frame);
    _chk_axis = new QCheckBox("Axis Equal", header_frame);
    sublayout->addWidget(_chk_grid);
    sublayout->addWidget(_chk_axis);
    header_layout->addRow("", subframe);

    layout->addWidget(header_frame);

    /* --------------------------- Barre d’outils plots ----------------------- */
    auto* toolbar = new QToolBar(this);
    toolbar->setIconSize(QSize(16, 16));
    toolbar->addAction(IhmTools::load_colored_icon(":/resources/appicons/add.svg"),     "New plot",   this, &GraphFrame::_cb_new_plot);
    auto* spacer = new QWidget(this); spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred); toolbar->addWidget(spacer);
    toolbar->addAction(IhmTools::load_colored_icon(":/resources/appicons/up.svg"),      "Up",         this, &GraphFrame::_cb_plot_up);
    toolbar->addAction(IhmTools::load_colored_icon(":/resources/appicons/down.svg"),    "Down",       this, &GraphFrame::_cb_plot_down);
    spacer = new QWidget(this); spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred); toolbar->addWidget(spacer);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addAction(IhmTools::load_colored_icon(":/resources/appicons/trash.svg"),   "Delete",     this, &GraphFrame::_cb_delete_plot);
    layout->addWidget(toolbar);

    /* ----------------------------- Liste des plots -------------------------- */
    _plots_list_model = new PlotListModel(this);
    _plots_list = new QListView(this);
    _plots_list->setIconSize(QSize(25, 25));
    _plots_list->setSelectionMode(QListView::SingleSelection);
    _plots_list->setModel(_plots_list_model);
    layout->addWidget(_plots_list);

    _main_frame->setVisible(false);
}
/* ---------------------- Affectation / mise à jour du graphe ---------------- */

void GraphFrame::set_graph_adapter(GraphAdapter* graph_adapter) {
    if (graph_adapter == _graph_adapter)
        return;
    _graph_adapter = graph_adapter;
    const bool visible = _graph_adapter && _graph_adapter->graph();
    _main_frame->setVisible(visible);
    if (!visible)
        return;
    _plots_list_model->set_adapter(_graph_adapter);
    update_from_values();
}
/**
 * @brief Synchronise les champs UI avec les valeurs du graphe actuel.
 */
void GraphFrame::update_from_values() {
    if (!_graph_adapter)
        return;

    // Sécurité : déconnecter d'abord pour éviter signaux multiples
    disconnect(_edt_title, nullptr, nullptr, nullptr);
    disconnect(_edt_xlabel, nullptr, nullptr, nullptr);
    disconnect(_edt_ylabel, nullptr, nullptr, nullptr);
    disconnect(_edt_zlabel, nullptr, nullptr, nullptr);
    disconnect(_chk_grid, nullptr, nullptr, nullptr);
    disconnect(_chk_axis, nullptr, nullptr, nullptr);
    disconnect(_plots_list->selectionModel(), &QItemSelectionModel::selectionChanged, nullptr, nullptr);

    _plots_list_model->update_model();

    // Mise à jour UI depuis l’adapter
    _edt_title->setText(_graph_adapter->title());
    _edt_xlabel->setText(_graph_adapter->x_label());
    _edt_ylabel->setText(_graph_adapter->y_label());
    _edt_zlabel->setText(_graph_adapter->z_label());
    _chk_grid->setChecked(_graph_adapter->grid());
    _chk_axis->setChecked(_graph_adapter->axis_equal());

    // Rétablir la sélection précédente
    const auto id = _graph_adapter->id();
    if (!_last_selected_plot.contains(id))
        _last_selected_plot[id] = 0;

    _plots_list->setCurrentIndex(_plots_list_model->index(_last_selected_plot[id]));
    const int i = _plots_list->currentIndex().row();
    _set_plotframe_plot(i);

    // Reconnexion des signaux
    connect(_edt_title,  &QLineEdit::editingFinished, [this]() { _graph_adapter->set_title(_edt_title->text()); });
    connect(_edt_xlabel, &QLineEdit::editingFinished, [this]() { _graph_adapter->set_x_label(_edt_xlabel->text()); });
    connect(_edt_ylabel, &QLineEdit::editingFinished, [this]() { _graph_adapter->set_y_label(_edt_ylabel->text()); });
    connect(_edt_zlabel, &QLineEdit::editingFinished, [this]() { _graph_adapter->set_z_label(_edt_zlabel->text()); });
    connect(_chk_grid, &QCheckBox::toggled, [this](bool checked) { _graph_adapter->set_grid(checked); });
    connect(_chk_axis, &QCheckBox::toggled, [this](bool checked) { _graph_adapter->set_axis_equal(checked); });
    connect(_plots_list->selectionModel(), &QItemSelectionModel::selectionChanged, this, &GraphFrame::_cb_plot_list_selection_changed);
}
QSize GraphFrame::sizeHint() const {
    return {300, 300};
}
/* ----------------------------- Slots internes ----------------------------- */

void GraphFrame::_cb_new_plot() {
    if (!_graph_adapter)
        return;

    _graph_adapter->add_plot();
    _plots_list_model->update_model();

    const short new_index = _graph_adapter->plots_count() - 1;
    _plots_list->setCurrentIndex(_plots_list_model->index(new_index, 0));
    _last_selected_plot[_graph_adapter->id()] = new_index;
    _plots_list->setFocus();
    _set_plotframe_plot(new_index);
}
void GraphFrame::_cb_plot_up() {
    int i = _plots_list->currentIndex().row();
    if (i > 0) {
        _graph_adapter->swap_plots(i, i - 1);
        _plots_list_model->update_model();
        _plots_list->setCurrentIndex(_plots_list_model->index(i - 1, 0));
        _last_selected_plot[_graph_adapter->id()] = i - 1;
    }
}
void GraphFrame::_cb_plot_down() {
    short i = _plots_list->currentIndex().row();
    if (i >= 0 && i < _graph_adapter->plots_count() - 1) {
        _graph_adapter->swap_plots(i, i + 1);
        _plots_list_model->update_model();
        _plots_list->setCurrentIndex(_plots_list_model->index(i + 1, 0));
        _last_selected_plot[_graph_adapter->id()] = i + 1;
    }
}
void GraphFrame::_cb_delete_plot() {
    short i = _plots_list->currentIndex().row();
    if (i < 0)
        return;

    _graph_adapter->delete_plot(i);
    _plots_list_model->update_model();

    short new_index = (i >= _graph_adapter->plots_count()) ? i - 1 : i;
    _plots_list->setCurrentIndex(_plots_list_model->index(new_index, 0));
    _last_selected_plot[_graph_adapter->id()] = new_index;

    _set_plotframe_plot(new_index);
}
void GraphFrame::_cb_plot_list_selection_changed() {
    const int i = _plots_list->currentIndex().row();
    if (i < 0)
        return;
    _set_plotframe_plot(i);
    _last_selected_plot[_graph_adapter->id()] = i;
}
void GraphFrame::_set_plotframe_plot(const short& idx) {
    _main_window->plot_frame()->set_plot_adapter(
        idx < 0 || idx >= _graph_adapter->plots_count() ? nullptr: std::make_unique<PlotAdapter>(&_graph_adapter->plot(idx), _main_window->plot_frame()));
}