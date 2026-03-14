#include "plot_frame.h"

StyleTableModel::StyleTableModel(QObject* parent): QStandardItemModel(parent), _plot_adapter(nullptr) {
    setColumnCount(2);
    setHorizontalHeaderLabels({ QString(), "Value" });
}
void StyleTableModel::set_plot_adapter(PlotAdapter* plot_adapter) {
    // if(plot_adapter == _plot_adapter) return;
    _plot_adapter = plot_adapter;
    setRowCount(_plot_adapter ? _style_database_adapter.style_items_count(_plot_adapter->type()): 0);
    update_model();
}
QVariant StyleTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || !_plot_adapter) return {};
    if(role == Qt::DisplayRole) {
        if(index.column() == 0) {
            QString label = _style_database_adapter.style_item_label(_plot_adapter->type(), index.row());
            return label;
        }
    }
    return {};
}
bool StyleTableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    return QStandardItemModel::setData(index, value, role);
}
void StyleTableModel::update_model() {
    beginResetModel();
    endResetModel();
}

QSize PlotFrame::sizeHint() const { return QSize(200, 500); }

PlotFrame::PlotFrame(MainWindow* parent): QFrame(parent), _main_window(parent), _plot_adapter(nullptr) {
    auto* layout = new QVBoxLayout(this);
    setLayout(layout);
    _header_frame = new QFrame(this);
    auto* header_layout = new QFormLayout(_header_frame);
    header_layout->setContentsMargins(0, 0, 0, 0);
    header_layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    _header_frame->setLayout(header_layout);
    _cbb_type = new QComboBox(this);
    for(unsigned int i=0; i<_plot_adapter->plot_type_count(); i++)
        _cbb_type->addItem(_plot_adapter->plot_type_label(i));
    _edt_label = new QLineEdit(this);
    _edt_label->setToolTip("Plot label");
    header_layout->addRow(_cbb_type, _edt_label);
    layout->addWidget(_header_frame);
    _header_frame->setVisible(false);

    _features_frame = new QFrame(this);
    _features_layout = new QFormLayout(_features_frame);
    _features_layout->setContentsMargins(0, 0, 0, 0);
    _features_layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    _features_frame->setLayout(_features_layout);
    layout->addWidget(_features_frame);
    _features_frame->setVisible(false);

    _style_table_model = new StyleTableModel(this);
    _style_table = new QTableView(this);
    _style_table->setModel(_style_table_model);
    layout->addWidget(_style_table);
    _style_table->setVisible(false);

}
void PlotFrame::set_plot_adapter(PlotAdapter* plot_adapter) {
    if(plot_adapter == _plot_adapter) return;
    _plot_adapter = plot_adapter;
    bool visible = _plot_adapter && _plot_adapter->plot();
    _header_frame->setVisible(visible);
    _features_frame->setVisible(visible);
    _style_table->setVisible(visible);
    if(visible) {
        disconnect(_edt_label, nullptr, nullptr, nullptr);
        disconnect(_cbb_type, nullptr, nullptr, nullptr);
        for(QLineEdit* le: _edt_feature)
            disconnect(le, nullptr, nullptr, nullptr);
        _cbb_type->setCurrentIndex(_plot_adapter->type_as_int());
        _edt_label->setText(_plot_adapter->label());
        _style_table_model->set_plot_adapter(_plot_adapter);
        _update_features();
        connect(_edt_label, &QLineEdit::editingFinished, this, &PlotFrame::_cb_label_changed);
        connect(_cbb_type, &QComboBox::currentIndexChanged, this, &PlotFrame::_cb_type_changed);
        for(unsigned int i = 0; i < _plot_adapter->features_count(); i++)
            connect(_edt_feature[i], &QLineEdit::editingFinished, this, [this, i](){this->_cb_feature_changed(i); });
    }
}
void PlotFrame::_update_features() {
    while(_features_layout->rowCount() > 0)
        _features_layout->removeRow(0);
    _edt_feature.clear();
    for(unsigned int i = 0; i < _plot_adapter->features_count(); i++) {
        QLineEdit* le = new QLineEdit(_features_frame);
        le->setText(_plot_adapter->feature(i));
        _edt_feature.append(le);
        _features_layout->addRow(_plot_adapter->feature_label(i), le);
    }
}
void PlotFrame::_cb_label_changed() {
    _plot_adapter->set_label(_edt_label->text());
    _main_window->graph_frame()->update_from_values();
}
void PlotFrame::_cb_type_changed() {
    _plot_adapter->set_type(_cbb_type->currentIndex());
    _style_table_model->set_plot_adapter(_plot_adapter);
    _update_features();
}
void PlotFrame::_cb_feature_changed(unsigned int idx) {
    _plot_adapter->set_feature_value(idx, _edt_feature[idx]->text());
}