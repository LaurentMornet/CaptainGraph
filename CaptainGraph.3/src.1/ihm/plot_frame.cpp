#include "plot_frame.h"
#include "ihm_tools.h"
#include <QLabel>

QSize PlotFrame::sizeHint() const { 
    return {200, 500}; 
}
PlotFrame::PlotFrame(MainWindow* parent): QFrame(parent), _main_window(parent), _plot_adapter(nullptr) {
    auto* initial_layout = new QVBoxLayout(this);
    initial_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(initial_layout);

    _main_frame = new QFrame(this);
    auto* layout = new QVBoxLayout(_main_frame);
    _main_frame->setLayout(layout);
    initial_layout->addWidget(_main_frame);

    // --- header (type + label)
    _header_frame = new QFrame(this);
    auto* header_layout = new QFormLayout(_header_frame);
    header_layout->setContentsMargins(0, 0, 0, 0);
    header_layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    _header_frame->setLayout(header_layout);
    _cbb_type = new QComboBox(this);
    _cbb_type->setFocusPolicy(Qt::StrongFocus);
    _cbb_type->setFixedHeight(30);
    _cbb_type->setIconSize(QSize(30, 30));
    _cbb_type->setAutoFillBackground(true);
    for(unsigned int i=0; i<PlotAdapter::plot_type_count(); i++) {
        _cbb_type->addItem(
            IhmTools::load_colored_icon(QString::fromStdString(Plot::__static__types_icons__[Plot::__static__plots_types_indexes__[i]])),
            ""
        );
        _cbb_type->setItemData(i, PlotAdapter::plot_type_label(i), Qt::ToolTipRole);
    }
    _edt_label = new QLineEdit(this);
    _edt_label->setToolTip("Plot label");
    header_layout->addRow(_cbb_type, _edt_label);
    layout->addWidget(_header_frame);

    // --- features
    _features_frame = new QFrame(this);
    _features_layout = new QFormLayout(_features_frame);
    _features_layout->setContentsMargins(0, 0, 0, 0);
    _features_layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    _features_frame->setLayout(_features_layout);
    layout->addWidget(_features_frame);

    // -- style
    _style_table = new StyleTableView(this);
    _style_table_model = new StyleTableModel(_style_table->style_database_adapter(), this);
    _style_table->setModel(_style_table_model);
    _style_table->verticalHeader()->hide();
    _style_table->horizontalHeader()->resizeSection(0, 150);
    _style_table->horizontalHeader()->setStretchLastSection(true);
    _style_table->setIconSize({80, 20});
    _style_delegate = new StyleDelegate(_style_table->style_database_adapter(), this);
    _style_table->setItemDelegate(_style_delegate);
    layout->addWidget(_style_table);
    connect(_style_table, &StyleTableView::firstItemBackTab, [=]() { focusPreviousChild(); });
    connect(_style_table, &StyleTableView::lastItemTab, [=]() { focusNextChild(); });

    _main_frame->setVisible(false);
}
void PlotFrame::set_plot_adapter(std::unique_ptr<PlotAdapter> plot_adapter) {
    if(plot_adapter && plot_adapter.get() == _plot_adapter.get()) return;
    _freeze = true;
    _plot_adapter = std::move(plot_adapter);
    bool visible = _plot_adapter && _plot_adapter->plot();
    _main_frame->setVisible(visible);
    if(visible) {
        // label
        disconnect(_edt_label, nullptr, nullptr, nullptr);
        _edt_label->setText(_plot_adapter->label());
        connect(_edt_label, &QLineEdit::textChanged, this, &PlotFrame::_cb_label_changed);
        // type
        disconnect(_cbb_type, nullptr, nullptr, nullptr);
        _cbb_type->setCurrentIndex(_plot_adapter->type_as_int());
        connect(_cbb_type, &QComboBox::currentIndexChanged, this, &PlotFrame::_cb_type_changed);
        // features
        for(QLineEdit* le: _edt_feature)
            disconnect(le, nullptr, nullptr, nullptr);
        _update_features();
        for(int i = 0; i < _plot_adapter->features_count(); i++)
            connect(_edt_feature[i], &QLineEdit::textChanged, this, [this, i](){this->_cb_feature_changed(i); });
        // style
        _style_table->set_plot_adapter(_plot_adapter.get());
        _style_table_model->set_plot_adapter(_plot_adapter.get());
        _style_delegate->set_plot_adapter(_plot_adapter.get());
    }
    _freeze = false;
}
void PlotFrame::_update_features() {
    // 1. Effacer les anciens widgets
    for (int r = _features_layout->rowCount() - 1; r >= 0; --r) {
        QWidget* label = _features_layout->itemAt(r, QFormLayout::LabelRole)->widget();
        QWidget* field = _features_layout->itemAt(r, QFormLayout::FieldRole)->widget();
        if (label) label->deleteLater();
        if (field) field->deleteLater();
        _features_layout->removeRow(r);
    }
    _edt_feature.clear();
    // 2. Reconstruire
    int count = _plot_adapter->features_count();
    for (int i=0; i<count; i++) {
        QLabel* lbl = new QLabel(_plot_adapter->feature_label(i), _features_frame);
        QLineEdit* edt = new QLineEdit(_plot_adapter->feature(i), _features_frame);
        _features_layout->addRow(lbl, edt);
        _edt_feature.append(edt);
        setTabOrder(i == 0 ? _edt_label: _edt_feature[i - 1], _edt_feature[i]);
    }
    if(count > 0)
        setTabOrder(_edt_feature[count - 1], _style_table);
}
void PlotFrame::_cb_label_changed() {
    if(_freeze) return;
    _plot_adapter->set_label(_edt_label->text());
    // un peu lourd: on reupdate tout le graph frame juste pour 
    // un plot label, mais ça simplifie la logique
    _main_window->graph_frame()->update_from_values();
}
void PlotFrame::_cb_type_changed() {
    if(_freeze) return;
    _plot_adapter->set_type(_cbb_type->currentIndex());
    _style_table_model->set_plot_adapter(_plot_adapter.get());
    // un peu lourd: on reupdate tout le graph frame juste pour 
    // un plot type, mais ça simplifie la logique
    _main_window->graph_frame()->update_from_values();
    _update_features();
}
void PlotFrame::_cb_feature_changed(unsigned int idx) {
    if(_freeze) return;
    _plot_adapter->set_feature_value(idx, _edt_feature[idx]->text());
}