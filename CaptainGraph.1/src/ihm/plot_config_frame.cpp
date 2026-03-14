#include "plot_config_frame.h"
#include <QAction>
#include <QLabel>
#include <QDebug>

#include "ihm_tools.h"

PlotConfigFrame * PlotConfigFrame::instance = nullptr;
PlotConfigFrame * PlotConfigFrame::get_instance(QWidget * parent) {
    if(!PlotConfigFrame::instance && parent)
        PlotConfigFrame::instance = new PlotConfigFrame(parent);
    return instance;
}
// PlotChangeCommand::PlotChangeCommand(Plot* plot, const std::string& key,
//     const StyleItemValue& old_value,
//     const StyleItemValue& new_value,
//     QAbstractItemModel* model,
//     const QModelIndex& index): _plot(plot), _key(key), _old_value(old_value), _new_value(new_value),
//           _model(model), _index(index) {
//     setText(QString("Modifier style \"%1\"").arg(QString::fromStdString(_plot->get_style_item(_key).label)));
// }
// PlotChangeCommand::PlotChangeCommand(
//     Plot* plot, 
//     const std::string& key,
//     const StyleItemValue& old_value,
//     const StyleItemValue& new_value,
//     int param): _style(false), _plot(plot), _key(key), _old_value(old_value), _new_value(new_value), _param(param) {
//     setText(QString("Modifier %1").arg(QString::fromStdString(key)));
// }
// PlotChangeCommand::PlotChangeCommand(Plot* plot, const std::string& key,
//     const std::map<std::string, StyleItemValue> old_value,
//     QAbstractItemModel* model,
//     const QModelIndex& index): _style(false), _plot(plot), _key(key), _old_styledef(old_value), _model(model), _index(index) {
//     setText(QString("Reset to defaults"));
// }
// void PlotChangeCommand::undo() {
//     if(_style) {
//         _plot->set_style_item_value(_key, _old_value);
//         emit _model->dataChanged(_index, _index);
//     }
//     else {
//         if(_key == "type") {
//             _plot->set_type(std::get<std::string>(_old_value));
//         }
//         else if(_key == "label") {
//             _plot->set_label(std::get<std::string>(_old_value));
//         }
//         else if(_key == "feature") {
//             _plot->set_feature_value(_param, std::get<std::string>(_old_value));
//         }
//         else if(_key == "reset_to_defaults") {
//             _plot->from_styledef(_old_styledef);
//             emit _model->dataChanged(_index, _index);
//         }
//     }
//     GraphConfigFrame::get_instance()->set_plot(_plot);
//     PlotConfigFrame::get_instance()->set_plot(_plot);
// }
// void PlotChangeCommand::redo() {
//     if(_style) {
//         _plot->set_style_item_value(_key, _new_value);
//         emit _model->dataChanged(_index, _index);
//     }
//     else {
//         if(_key == "type") {
//             _plot->set_type(std::get<std::string>(_new_value));
//         }
//         else if(_key == "label") {
//             _plot->set_label(std::get<std::string>(_new_value));
//         }
//         else if(_key == "feature") {
//             _plot->set_feature_value(_param, std::get<std::string>(_new_value));
//         }
//         else if(_key == "reset_to_defaults") {
//             _plot->raz_styledef();
//             emit _model->dataChanged(_index, _index);
//         }
//     }
// }

/* ========================================================================== */
/*                        StyleDelegate implémentation                        */
/* ========================================================================== */

StyleDelegate::StyleDelegate(PlotConfigFrame* frame, QObject* parent)
    : QStyledItemDelegate(parent), _frame(frame) {}
QWidget* StyleDelegate::createEditor(QWidget* parent,
                                     const QStyleOptionViewItem& option,
                                     const QModelIndex& index) const {
    Q_UNUSED(option)
    if (index.column() != 1 || !_frame)
        return nullptr;

    auto ctx = _frame->context_for_index(index);
    if (!ctx.plot)
        return nullptr;

    switch (ctx.type) {
        case StyleItemType::STRING: {
            auto* le = new QLineEdit(parent);
            return le;
        }
        case StyleItemType::INTEGER: {
            auto* le = new QLineEdit(parent);
            le->setValidator(new QIntValidator(INT_MIN, INT_MAX, le));
            return le;
        }
        case StyleItemType::DOUBLE: {
            auto* le = new QLineEdit(parent);
            auto* dvalidator = new QDoubleValidator(-DBL_MAX, DBL_MAX, 6, le);
            dvalidator->setLocale(QLocale::C);
            le->setValidator(dvalidator);
            return le;
        }
        case StyleItemType::CUSTOM_ICON: {
            auto* cb = new QComboBox(parent);
            _frame->_populate_icon_combo(cb, std::get<std::string>(ctx.args[0]), QSize(std::get<int>(ctx.args[1]), std::get<int>(ctx.args[2])));
            return cb;
        }
        case StyleItemType::INT_RANGE: {
            auto* sb = new QSpinBox(parent);
            sb->setRange(std::get<int>(ctx.args[0]), std::get<int>(ctx.args[1]));
            return sb;
        }
        case StyleItemType::DOUBLE_RANGE: {
            auto* dsb = new QDoubleSpinBox(parent);
            dsb->setRange(std::get<double>(ctx.args[0]), std::get<double>(ctx.args[1]));
            dsb->setSingleStep(std::get<double>(ctx.args[2]));
            dsb->setLocale(QLocale::C);
            return dsb;
        }
        case StyleItemType::COLOR: {
            auto* cdlg = new QColorDialog(parent);
            cdlg->setOption(QColorDialog::ShowAlphaChannel, true);
            return cdlg;
        }
        default:
            return nullptr;
    }
}
void StyleDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    if (!_frame)
        return;
    auto ctx = _frame->context_for_index(index);
    if (!ctx.plot)
        return;

    switch (ctx.type) {
        case StyleItemType::STRING:
            if (auto* le = qobject_cast<QLineEdit*>(editor))
                le->setText(QString::fromStdString(std::get<std::string>(ctx.value)));
            break;
        case StyleItemType::INTEGER:
            if (auto* le = qobject_cast<QLineEdit*>(editor))
                le->setText(QString::number(std::get<int>(ctx.value)));
            break;
        case StyleItemType::DOUBLE:
            if (auto* le = qobject_cast<QLineEdit*>(editor))
                le->setText(QString::number(std::get<double>(ctx.value), 'f', 6));
            break;
        case StyleItemType::CUSTOM_ICON:
            if (auto* cb = qobject_cast<QComboBox*>(editor))
                cb->setCurrentIndex(std::get<int>(ctx.value));
            break;
        case StyleItemType::INT_RANGE:
            if (auto* sb = qobject_cast<QSpinBox*>(editor))
                sb->setValue(std::get<int>(ctx.value));
            break;
        case StyleItemType::DOUBLE_RANGE:
            if (auto* dsb = qobject_cast<QDoubleSpinBox*>(editor))
                dsb->setValue(std::get<double>(ctx.value));
            break;
        case StyleItemType::COLOR:
            if (auto* cdlg = qobject_cast<QColorDialog*>(editor)) {
                const QColor color(QString::fromStdString(std::get<std::string>(ctx.value)));
                if (color.isValid())
                    cdlg->setCurrentColor(color);
            }
            break;
        default:
            break;
    }
}
void StyleDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                 const QModelIndex& index) const {
    QVariant new_value;
    if (auto* le = qobject_cast<QLineEdit*>(editor))
        new_value = le->text();
    else if (auto* cb = qobject_cast<QComboBox*>(editor))
        new_value = cb->currentIndex();
    else if (auto* sb = qobject_cast<QSpinBox*>(editor))
        new_value = sb->value();
    else if (auto* dsb = qobject_cast<QDoubleSpinBox*>(editor))
        new_value = dsb->value();
    else if (auto* cdlg = qobject_cast<QColorDialog*>(editor))
        new_value = cdlg->currentColor();
    model->setData(index, new_value, Qt::EditRole);
}
void StyleDelegate::updateEditorGeometry(QWidget* editor,
                                         const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const {
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}
/* ========================================================================== */
/*                        StyleTableModel implémentation                      */
/* ========================================================================== */
StyleTableModel::StyleTableModel(PlotConfigFrame* frame)
    : QStandardItemModel(frame), _frame(frame) {
    setColumnCount(2);
    setHorizontalHeaderLabels({"Style Item", "Value"});
}
void StyleTableModel::set_style_data() {
    if (!_frame)
        return;
    auto* plot = &_frame->_buffer_plots[_frame->_ref_plot->type()];
    setRowCount(static_cast<int>(plot->get_style_keys().size()));
}
QVariant StyleTableModel::data(const QModelIndex & index, int role) const {
    if (!_frame || !_frame->_ref_plot || !index.isValid())
        return QVariant();

    const auto ctx = _frame->context_for_index(index);
    if (!ctx.plot)
        return QVariant();
    const int col = index.column();

    // === Texte principal ===
    if (role == Qt::DisplayRole) {
        if (col == 0)
            return QString::fromStdString(ctx.label);

        if (col == 1) {
            try {
                switch (ctx.type) {
                    case StyleItemType::STRING:
                        return QString::fromStdString(std::get<std::string>(ctx.value));
                    case StyleItemType::INTEGER:
                    case StyleItemType::INT_RANGE:
                        return std::get<int>(ctx.value);
                    case StyleItemType::DOUBLE:
                    case StyleItemType::DOUBLE_RANGE:
                        return QString::number(std::get<double>(ctx.value), 'f', 6);
                    default:
                        return QVariant();
                }
            } catch (...) {
                return QVariant();
            }
        }
    }

    // === Décoration ===
    if (role == Qt::DecorationRole && col == 1) {
        try {
            switch (ctx.type) {
                case StyleItemType::CUSTOM_ICON: {
                    const int idx = std::get<int>(ctx.value);
                    if (idx >= 0 && static_cast<size_t>(idx) < Plot::CustomIconTypes[std::get<std::string>(ctx.args[0])].size())
                        return IhmTools::load_colored_icon(QString("%1.svg")
                            .arg(Plot::CustomIconTypes[std::get<std::string>(ctx.args[0])][idx].c_str()));
                    break;
                }
                case StyleItemType::COLOR: {
                    const std::string s = std::get<std::string>(ctx.value);
                    const QColor color_val(QString::fromStdString(s));
                    if (color_val.isValid())
                        return color_val;
                    break;
                }
                default:
                    break;
            }
        } catch (...) {
            return QVariant();
        }
    }

    // === Checkbox ===
    if (role == Qt::CheckStateRole && col == 1 && ctx.type == StyleItemType::BOOL) {
        try {
            return std::get<bool>(ctx.value) ? Qt::Checked : Qt::Unchecked;
        } catch (...) {
            return QVariant();
        }
    }

    return QVariant();
}
bool StyleTableModel::setData(const QModelIndex & index, const QVariant & value, int role) {
    if (!_frame || !_frame->_ref_plot || !index.isValid())
        return false;

    Plot& plot = _frame->_buffer_plots[_frame->_ref_plot->type()];
    const auto ctx = _frame->context_for_index(index);
    if (ctx.key.empty())
        return false;

    StyleItemValue old_value = ctx.value;
    StyleItemValue new_value;

    try {
        if (role == Qt::CheckStateRole && ctx.type == StyleItemType::BOOL) {
            const bool checked = (value.toInt() == Qt::Checked);
            new_value = StyleItemValue(checked);
            plot.set_style_item_value(ctx.key, new_value);
            emit dataChanged(index, index, {Qt::CheckStateRole});
            _frame->_ref_plot->from(plot);
            return true;
        }

        if (role == Qt::EditRole) {
            switch (ctx.type) {
                case StyleItemType::CUSTOM_ICON:
                case StyleItemType::INTEGER:
                case StyleItemType::INT_RANGE:
                    new_value = StyleItemValue(value.toInt());
                    plot.set_style_item_value(ctx.key, StyleItemValue(new_value));
                    break;
                case StyleItemType::DOUBLE:
                case StyleItemType::DOUBLE_RANGE:
                    new_value = StyleItemValue(value.toDouble());
                    plot.set_style_item_value(ctx.key, new_value);
                    break;
                case StyleItemType::STRING:
                    new_value = StyleItemValue(value.toString().toStdString());
                    plot.set_style_item_value(ctx.key, new_value);
                    break;
                case StyleItemType::COLOR:
                    if (value.canConvert<QColor>()) {
                        const QString name = value.value<QColor>().name();
                        new_value = StyleItemValue(name.toStdString());
                        plot.set_style_item_value(ctx.key, new_value);
                    }
                    break;
                default:
                    break;
            }
            // emit dataChanged(index, index, {Qt::EditRole});
            // UndoManager::push(new PlotChangeCommand(_frame->_ref_plot, ctx.key, old_value, new_value, this, index));
            return true;
        }
    } catch (const std::exception& e) {
        qWarning() << "setData error:" << e.what();
        return false;
    }

    return QStandardItemModel::setData(index, value, role);
}
Qt::ItemFlags StyleTableModel::flags(const QModelIndex & index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    const auto ctx = _frame ? _frame->context_for_index(index) : PlotContext{};
    Qt::ItemFlags flags = QStandardItemModel::flags(index);

    if (index.column() == 1) {
        if (ctx.type == StyleItemType::BOOL)
            flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
        else
            flags |= Qt::ItemIsEditable;
    }
    return flags;
}
/* ========================================================================== */
/*                          PlotConfigFrame implémentation                    */
/* ========================================================================== */
QSize PlotConfigFrame::sizeHint() const {
    return QSize(300, 300);
}
PlotConfigFrame::PlotConfigFrame(QWidget* parent)
    : QFrame(parent), _ref_plot(nullptr), _inhibit_type_callback(false) {

    // setMinimumWidth(250);

    // for(auto item: Plot::PlotTypeNames) {
    //     _buffer_plots[item] = Plot();
    //     _buffer_plots[item].set_type(item);
    //     _buffer_plots[item].from(Plot::model_for_type(item));
    // }

    // auto* layout_main = new QVBoxLayout(this);
    // setLayout(layout_main);

    // /* --- Toolbar --- */
    // _toolbar = new QToolBar(this);
    // _toolbar->setIconSize(QSize(16, 16));

    // auto* action_reset_defaults = _toolbar->addAction(
    //     IhmTools::load_colored_icon(":/resources/appicons/refresh.svg"), "Reset to defaults");
    // connect(action_reset_defaults, &QAction::triggered, this, &PlotConfigFrame::_cb_reset_defaults);

    // auto* spacer = new QWidget(this);
    // spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    // _toolbar->addWidget(spacer);

    // auto* action_delete_plot = _toolbar->addAction(
    //     IhmTools::load_colored_icon(":/resources/appicons/trash.svg"), "Delete this plot");
    // connect(action_delete_plot, &QAction::triggered, this, &PlotConfigFrame::_cb_delete_plot);

    // layout_main->addWidget(_toolbar);

    // /* --- Header --- */
    // _box_header = new QFrame(this);
    // auto* layout_header = new QFormLayout(_box_header);
    // layout_header->setContentsMargins(0, 0, 0, 0);
    // layout_header->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    // _edt_label = new QLineEdit("", this);
    // connect(_edt_label, &QLineEdit::editingFinished, this, &PlotConfigFrame::_cb_label_changed);

    // _cb_type = new QComboBox(this);
    // _cb_type->setFixedHeight(30);
    // connect(_cb_type, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //         this, &PlotConfigFrame::_cb_type_changed);
    // _populate_icon_combo(_cb_type, "plot_type", QSize(30, 30));

    // auto * subframe = new QFrame(this);
    // auto * subframe_layout = new QFormLayout(subframe);
    // subframe_layout->setContentsMargins(0, 0, 0, 0);
    // subframe_layout->addRow(_cb_type, new QLabel("Label", this));
    // layout_header->addRow(subframe, _edt_label);

    // layout_main->addWidget(_box_header);

    // /* --- Features --- */
    // _box_features = new QFrame(this);
    // auto* layout_features = new QFormLayout(_box_features);
    // layout_features->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    // _box_features->setLayout(layout_features);
    // layout_main->addWidget(_box_features);

    // /* --- Style table --- */
    // _table_style_model = new StyleTableModel(this);
    // _table_style = new QTableView(this);
    // layout_main->addWidget(_table_style);
    // _table_style->setModel(_table_style_model);
    // _table_style->setItemDelegate(new StyleDelegate(this, this));
    // _table_style->horizontalHeader()->setStretchLastSection(true);
    // _table_style->horizontalHeader()->resizeSection(0, 150);
    // _table_style->verticalHeader()->hide();
    // _table_style->setIconSize(QSize(80, 20));

    // _toolbar->setVisible(false);
    // _box_header->setVisible(false);
    // _box_features->setVisible(false);
    // _table_style->setVisible(false);
}
void PlotConfigFrame::_populate_icon_combo(QComboBox* combo,
                                           const std::string& custom_icon_map_id,
                                           const QSize& icon_size) const {
    if (!combo)
        return;

    combo->clear();
    combo->setIconSize(icon_size);
    combo->setAutoFillBackground(true);

    for (const auto& icon_path : Plot::CustomIconTypes[custom_icon_map_id]) {
        const QString path = QString("%1.svg").arg(icon_path.c_str());
        combo->addItem(IhmTools::load_colored_icon(path), "");
    }
}
PlotContext PlotConfigFrame::context_for_index(const QModelIndex& index) const {
    PlotContext ctx;
    // if (!_ref_plot)
    //     return ctx;

    // ctx.plot = const_cast<Plot*>(&_buffer_plots.at(_ref_plot->type()));
    // ctx.keys = ctx.plot->get_style_keys();
    // if (index.row() >= 0 && index.row() < static_cast<int>(ctx.keys.size())) {
    //     ctx.key = ctx.keys[index.row()];
    //     auto item = ctx.plot->get_style_item(ctx.key);
    //     ctx.label = item.label;
    //     ctx.type = item.type;
    //     if (auto val = ctx.plot->get_style_item_value(ctx.key))
    //         ctx.value = val.value();
    //     if (auto args = ctx.plot->get_style_item_args(ctx.key))
    //         ctx.args = args.value();
    // }
    return ctx;
}
void PlotConfigFrame::set_plot_Adapter(PlotAdapter* plot_adapter) {
    // if (!plot_Adapter) {
    //     _toolbar->setVisible(false);
    //     _box_header->setVisible(false);
    //     _box_features->setVisible(false);
    //     _table_style->setVisible(false);
    //     return;
    // }
    // _inhibit_type_callback = true;
    // _toolbar->setVisible(true);
    // _box_header->setVisible(true);
    // _box_features->setVisible(true);
    // _table_style->setVisible(true);
    // _ref_plot = plot;
    // _edt_label->setText(QString::fromStdString(_ref_plot->label()));
    // for (auto& [_, plot] : _buffer_plots)
    //     plot.raz_styledef();
    // _buffer_plots[_ref_plot->type()].from(*_ref_plot);
    // auto v = Plot::PlotTypeNames;
    // auto index = std::find(v.begin(), v.end(), _ref_plot->type()) - v.begin();
    // if(_cb_type->currentIndex() != index)
    //     _cb_type->setCurrentIndex(index);
    // _table_style_model->set_style_data();
    // _update_features_form();
    // _inhibit_type_callback = false;
}
void PlotConfigFrame::_update_features_form() {
    // auto* layout = qobject_cast<QFormLayout*>(_box_features->layout());
    // size_t i = 0;
    // for (; i < _ref_plot->features_count(); i++) {
    //     if(i >= (size_t)_edt_features.size()) {
    //         auto* le = new QLineEdit(QString::fromStdString(_ref_plot->feature_value(i)), _box_features);
    //         auto* lbl = new QLabel(QString::fromStdString(_ref_plot->feature_name(i)), this);
    //         _edt_features.append(le);
    //         _lbl_features.append(lbl);
    //         connect(le, &QLineEdit::editingFinished, this,
    //             [this, i, le]() {
    //                 QString texte = le->text();
    //                 _cb_feature_changed(i, texte);
    //             });
    //         layout->addRow(lbl, le);
    //     }
    //     else {
    //         _edt_features[i]->setText(QString::fromStdString(_ref_plot->feature_value(i)));
    //         _lbl_features[i]->setText(QString::fromStdString(_ref_plot->feature_name(i)));
    //     }
    // }
    // for(; i < (size_t)layout->rowCount(); i++) {
    //     _edt_features.pop_back();
    //     _lbl_features.pop_back();
    //     layout->removeRow(i);
    // }
}
void PlotConfigFrame::_cb_type_changed(int index) {
    // if (_inhibit_type_callback) return;
    // if (!_ref_plot) return;
    // _ref_plot->set_type(Plot::PlotTypeNames[index]);
    // // std::string old_value = _ref_plot->type();
    // // std::string new_value = Plot::PlotTypeNames[index];
    // // UndoManager::push(new PlotChangeCommand(_ref_plot, "type", old_value, new_value, 0));
}
void PlotConfigFrame::_cb_reset_defaults() {
    // if(!_ref_plot) return;
    // _ref_plot->raz_styledef();
    // // std::map<std::string, StyleItemValue> old_styledef = _ref_plot->current_styledef();
    // // UndoManager::push(new PlotChangeCommand(_ref_plot, "reset_to_defaults", old_styledef, _table_style_model, QModelIndex()));
}
void PlotConfigFrame::_cb_label_changed() {
    // if (!_ref_plot) return;
    // // std::string old_value = _ref_plot->label();
    // // std::string new_value = _edt_label->text().toStdString();
    // // if(old_value == new_value) return;
    // _ref_plot->set_label(_edt_label->text());
    // // UndoManager::push(new PlotChangeCommand(_ref_plot, "label", old_value, new_value, 0));
}
void PlotConfigFrame::_cb_feature_changed(int index, QString value) {
    // if (!_ref_plot) return;
    // // std::string old_value = _ref_plot->feature_value(index);
    // // std::string new_value = value.toStdString();
    // // if(old_value == new_value) return;
    // _ref_plot->set_feature_value(index, value);
    // // UndoManager::push(new PlotChangeCommand(_ref_plot, "feature", old_value, new_value, index));
}
void PlotConfigFrame::_cb_delete_plot() {}