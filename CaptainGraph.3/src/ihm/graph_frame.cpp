#include "graph_frame.h"
#include <QDebug>
#include <QToolBar>
#include <QScrollArea>

/* ----------------------------- Plot List Model ------------------------ */
PlotListModel::PlotListModel(QObject* parent): QAbstractListModel(parent), _adapter(nullptr) {}
void PlotListModel::set_adapter(GraphAdapter* adapter) {
    _adapter = adapter;
    update_model();
}
int PlotListModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent)
    return _adapter ? _adapter->plots_count() : 0;
}
QVariant PlotListModel::data(const QModelIndex & index, int role) const {
    if(_adapter) {
        if(role == Qt::DisplayRole) {
            return _adapter->plot_label(index.row());
        }
        else if(role == Qt::DecorationRole) {
            // return IhmTools::load_colored_icon(QString(":/resources/plottypes/%1.svg").arg(_frame->_ref_graph->plot(index.row()).type().c_str()));
        }
    }
    return QVariant();
}
void PlotListModel::update_model() {
    beginResetModel();
    endResetModel();
}
/* ----------------------------- GraphFrame ----------------------------- */
GraphFrame::GraphFrame(MainWindow* parent)
    : QFrame(parent), _main_window(parent), _graph_adapter(nullptr) {
    auto* main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(main_layout);

    _main_frame = new QFrame(this);
    main_layout->addWidget(_main_frame);
    auto* layout = new QVBoxLayout(_main_frame);

    auto* header_frame = new QFrame(this);
    auto* header_layout = new QFormLayout(header_frame);
    header_layout->setContentsMargins(0, 0, 0, 0);
    header_layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    header_frame->setLayout(header_layout);

    _edt_title = new QLineEdit(header_frame);
    header_layout->addRow("Title", _edt_title);
    _edt_xlabel = new QLineEdit(header_frame);
    header_layout->addRow("X Label", _edt_xlabel);
    _edt_ylabel = new QLineEdit(header_frame);
    header_layout->addRow("Y Label", _edt_ylabel);
    _edt_zlabel = new QLineEdit(header_frame);
    header_layout->addRow("Z Label", _edt_zlabel);
    auto * subframe = new QFrame(header_frame);
    subframe->setLayout(new QHBoxLayout(subframe));
    subframe->layout()->setContentsMargins(0, 0, 0, 0);
    _chk_grid = new QCheckBox("Grid", header_frame); _chk_grid->setTristate(false); _chk_grid->setFocusPolicy(Qt::StrongFocus);
    _chk_axis = new QCheckBox("Axis Equal", header_frame); _chk_axis->setTristate(false); _chk_axis->setFocusPolicy(Qt::StrongFocus);
    subframe->layout()->addWidget(_chk_grid);
    subframe->layout()->addWidget(_chk_axis);
    header_layout->addRow("", subframe);

    layout->addWidget(header_frame);

    // Gestion des plots: toolbar
    auto* toolbar = new QToolBar(this);
    auto* action = toolbar->addAction("New plot");
    connect(action, &QAction::triggered, this, &GraphFrame::_cb_new_plot);
    action = toolbar->addAction("Up");
    connect(action, &QAction::triggered, this, &GraphFrame::_cb_plot_up);
    action = toolbar->addAction("Down");
    connect(action, &QAction::triggered, this, &GraphFrame::_cb_plot_down);
    action = toolbar->addAction("Delete");
    connect(action, &QAction::triggered, this, &GraphFrame::_cb_delete_plot);
    layout->addWidget(toolbar);
    // Liste des plots
    _plots_list_model = new PlotListModel(this);
    _plots_list = new QListView(this);
    _plots_list->setSelectionMode(QListView::SingleSelection);
    _plots_list->setModel(_plots_list_model);
    layout->addWidget(_plots_list);
    // Initialement la frame n'est pas visible (pas d'adapter connecté)
    _main_frame->setVisible(false);
}

GraphFrame::~GraphFrame() {
    _graph_adapter = nullptr;
}

void GraphFrame::set_graph_adapter(GraphAdapter* graph_adapter) {
    if (graph_adapter == _graph_adapter)
        return;
    _graph_adapter = graph_adapter;
    
    bool visible = _graph_adapter && _graph_adapter->graph();
    _main_frame->setVisible(visible);
    if(visible) {
        _main_window->plot_frame()->set_plot_adapter(nullptr);
        _plots_list_model->set_adapter(_graph_adapter);
        update_from_values();
    }
}

void GraphFrame::update_from_values() {
    disconnect(_edt_title, nullptr, nullptr, nullptr);
    disconnect(_edt_xlabel, nullptr, nullptr, nullptr);
    disconnect(_edt_ylabel, nullptr, nullptr, nullptr);
    disconnect(_edt_zlabel, nullptr, nullptr, nullptr);
    disconnect(_chk_grid, nullptr, nullptr, nullptr);
    disconnect(_chk_axis, nullptr, nullptr, nullptr);
    disconnect(_plots_list->selectionModel(), &QItemSelectionModel::selectionChanged, nullptr, nullptr);
    _plots_list_model->update_model();
    _edt_title->setText(_graph_adapter->title());
    _edt_xlabel->setText(_graph_adapter->x_label());
    _edt_ylabel->setText(_graph_adapter->y_label());
    _edt_zlabel->setText(_graph_adapter->z_label());
    _chk_grid->setChecked(_graph_adapter->grid());
    _chk_axis->setChecked(_graph_adapter->axis_equal());
    if(!_last_selected_plot.contains(_graph_adapter->id()))
        _last_selected_plot[_graph_adapter->id()] = 0;
    _plots_list->setCurrentIndex(_plots_list_model->index(_last_selected_plot[_graph_adapter->id()]));
    int i = _plots_list->currentIndex().row();
    if(i >= 0) _main_window->plot_frame()->set_plot_adapter(new PlotAdapter(&_graph_adapter->plot(i)));
    connect(_edt_title, &QLineEdit::editingFinished, this, [this]() { this->_graph_adapter->set_title(this->_edt_title->text()); });
    connect(_edt_xlabel, &QLineEdit::editingFinished, this, [this]() { this->_graph_adapter->set_x_label(this->_edt_xlabel->text()); });
    connect(_edt_ylabel, &QLineEdit::editingFinished, this, [this]() { this->_graph_adapter->set_y_label(this->_edt_ylabel->text()); });
    connect(_edt_zlabel, &QLineEdit::editingFinished, this, [this]() { this->_graph_adapter->set_z_label(this->_edt_zlabel->text()); });
    connect(_chk_grid, &QCheckBox::stateChanged, this, [this]() { this->_graph_adapter->set_grid(this->_chk_grid->isChecked()); });
    connect(_chk_axis, &QCheckBox::stateChanged, this, [this]() { this->_graph_adapter->set_axis_equal(this->_chk_axis->isChecked()); });
    connect(_plots_list->selectionModel(), &QItemSelectionModel::selectionChanged, this, &GraphFrame::_cb_plot_list_selection_changed);
}

QSize GraphFrame::sizeHint() const {
    return QSize(300, 300);
}

void GraphFrame::_cb_new_plot() {
    if(_graph_adapter) {
        _graph_adapter->add_plot();
        _plots_list_model->update_model();
        _plots_list->setCurrentIndex(_plots_list_model->index(_graph_adapter->plots_count() - 1, 0));
        _last_selected_plot[_graph_adapter->id()] = _graph_adapter->plots_count() - 1;
    }
}
void GraphFrame::_cb_plot_up() {
    int i = _plots_list->currentIndex().row();
    if(i > 0) {
        _graph_adapter->swap_plots(i, i - 1);
        _plots_list_model->update_model();
        _plots_list->setCurrentIndex(_plots_list_model->index(i - 1, 0));
        _last_selected_plot[_graph_adapter->id()] = i - 1;
    }
}
void GraphFrame::_cb_plot_down() {
    int i = _plots_list->currentIndex().row();
    if(i >= 0 && (unsigned int)i < _graph_adapter->plots_count() - 1) {
        _graph_adapter->swap_plots(i, i + 1);
        _plots_list_model->update_model();
        _plots_list->setCurrentIndex(_plots_list_model->index(i + 1, 0));
        _last_selected_plot[_graph_adapter->id()] = i + 1;
    }
}
void GraphFrame::_cb_delete_plot() {
    int i = _plots_list->currentIndex().row();
    if(i >= 0) {
        _graph_adapter->delete_plot(i);
        _plots_list_model->update_model();
        _plots_list->setCurrentIndex(_plots_list_model->index((unsigned int)i >= _graph_adapter->plots_count() ? i-1: i, 0));
        _last_selected_plot[_graph_adapter->id()] = (unsigned int)i >= _graph_adapter->plots_count() ? i-1: i;
        i = _plots_list->currentIndex().row();
        _main_window->plot_frame()->set_plot_adapter(i >= 0 ? new PlotAdapter(&_graph_adapter->plot(i)) : nullptr);
    }
}

void GraphFrame::_cb_plot_list_selection_changed() {
    int i = _plots_list->currentIndex().row();
    if(i >= 0) {
        _main_window->plot_frame()->set_plot_adapter(
            new PlotAdapter(&_graph_adapter->plot(i))
        );
        _last_selected_plot[_graph_adapter->id()] = _plots_list->currentIndex().row();
    }
}