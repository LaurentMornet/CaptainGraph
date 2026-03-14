#include "graph_config_frame.h"
#include "ihm_tools.h"

GraphConfigFrame * GraphConfigFrame::instance = nullptr;
GraphConfigFrame * GraphConfigFrame::get_instance(QWidget * parent) {
    if(!GraphConfigFrame::instance && parent)
        GraphConfigFrame::instance = new GraphConfigFrame(parent);
    return instance;
}
PlotListModel::PlotListModel(GraphConfigFrame * frame) : QAbstractListModel(), _frame(frame) {}
void PlotListModel::set_list_data() {
    beginResetModel();
    endResetModel();
}
int PlotListModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent)
    return _frame->_ref_graph->plots_count();
}
QVariant PlotListModel::data(const QModelIndex & index, int role) const {
    if(role == Qt::DisplayRole) {
        return QString(_frame->_ref_graph->plot(index.row()).label().c_str());
    }
    else if(role == Qt::DecorationRole) {
        return IhmTools::load_colored_icon(QString(":/resources/plottypes/%1.svg").arg(_frame->_ref_graph->plot(index.row()).type().c_str()));
    }
    return QVariant();
}
QSize GraphConfigFrame::sizeHint() const {
    return QSize(100, 100);
}
GraphConfigFrame::GraphConfigFrame(QWidget * parent) : QFrame(parent), _ref_graph(nullptr), _list_plots_model(this) {
    auto* layout_main = new QVBoxLayout(this);
    setLayout(layout_main);

    /* --- Toolbar --- */
    _toolbar = new QToolBar(this);
    _toolbar->setIconSize(QSize(16, 16));

    auto* action_new_plot = _toolbar->addAction(
        IhmTools::load_colored_icon(":/resources/appicons/add.svg"), "New plot");
    connect(action_new_plot, &QAction::triggered, this, &GraphConfigFrame::_cb_new_plot);

    auto* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    _toolbar->addWidget(spacer);

    auto* action_up = _toolbar->addAction(
        IhmTools::load_colored_icon(":/resources/appicons/up.svg"), "Up");
    connect(action_up, &QAction::triggered, this, &GraphConfigFrame::_cb_up);
    auto* action_down = _toolbar->addAction(
        IhmTools::load_colored_icon(":/resources/appicons/down.svg"), "Down");
    connect(action_down, &QAction::triggered, this, &GraphConfigFrame::_cb_down);
    
    spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    _toolbar->addWidget(spacer);

    auto* action_del_plot = _toolbar->addAction(
        IhmTools::load_colored_icon(":/resources/appicons/trash.svg"), "Delete plot");
    connect(action_del_plot, &QAction::triggered, this, &GraphConfigFrame::_cb_delete_plot);

    _box_header = new QFrame(this);
    auto* layout_header = new QFormLayout(_box_header);
    layout_header->setContentsMargins(0, 0, 0, 0);
    layout_header->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    _edt_title = new QLineEdit("", this);
    connect(_edt_title, &QLineEdit::editingFinished, this, &GraphConfigFrame::_cb_title_changed);
    layout_header->addRow("Title", _edt_title);

    _list_plots = new QListView(this);
    _list_plots->setModel(&_list_plots_model);
    connect(_list_plots, &QListView::pressed, this, &GraphConfigFrame::_cb_list_pressed);

    _box_params = new QFrame(this);
    auto* layout_params = new QFormLayout(_box_params);
    layout_params->setContentsMargins(0, 0, 0, 0);
    layout_params->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    auto * chk = new QCheckBox(this);
    chk->setTristate(false);
    connect(chk, &QCheckBox::stateChanged, this, &GraphConfigFrame::_cb_axis_equal_checked);
    layout_params->addRow("Axis equal", chk);
    chk = new QCheckBox(this);
    chk->setTristate(false);
    connect(chk, &QCheckBox::stateChanged, this, &GraphConfigFrame::_cb_grid_checked);
    layout_params->addRow("Grid on", chk);

    layout_main->addWidget(_box_header);
    layout_main->addWidget(_toolbar);
    layout_main->addWidget(_list_plots);
    layout_main->addWidget(_box_params);

    _toolbar->setVisible(false);
    _box_header->setVisible(false);
    _list_plots->setVisible(false);
    _box_params->setVisible(false);
}
void GraphConfigFrame::set_graph(Graph * graph) {
    if (!graph) {
        _toolbar->setVisible(false);
        _box_header->setVisible(false);
        _list_plots->setVisible(false);
        _box_params->setVisible(false);
        return;
    }
    _ref_graph = graph;
    _toolbar->setVisible(true);
    _box_header->setVisible(true);
    _list_plots->setVisible(true);
    _box_params->setVisible(true);
    _edt_title->setText(QString::fromStdString(_ref_graph->title()));
    _list_plots_model.set_list_data();
}
void GraphConfigFrame::set_plot(Plot * plot) {
    if(!plot) return;
    set_graph(plot->graph());
    int row = _ref_graph->index_of_plot(plot);
    if(row >= 0) {
        QModelIndex idx = _list_plots_model.index(row, 0);
        _list_plots->setCurrentIndex(idx);
    }
}
void GraphConfigFrame::_cb_title_changed() {
    _ref_graph->set_title(_edt_title->text().toStdString());
}
void GraphConfigFrame::_cb_axis_equal_checked(bool checked) {
    Q_UNUSED(checked)
}
void GraphConfigFrame::_cb_grid_checked(bool checked) {
    Q_UNUSED(checked)
}
void GraphConfigFrame::_cb_new_plot() {
    _ref_graph->add_plot();
    _list_plots_model.set_list_data();
}
void GraphConfigFrame::_cb_delete_plot() {
    int row = _list_plots->currentIndex().row();
    _ref_graph->remove_plot(&_ref_graph->plot(row));
    _list_plots_model.set_list_data();
}
void GraphConfigFrame::_cb_list_pressed(const QModelIndex & index) {
    PlotConfigFrame::get_instance()->set_plot(&_ref_graph->plot(index.row()));
}
void GraphConfigFrame::_cb_up() {}
void GraphConfigFrame::_cb_down() {}