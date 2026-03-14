#include "plot_style.h"
#include "ihm_tools.h"
/** 
 * StyleTableView
 * --------------
 * Classe dérivée de QTableView pour gestion du focus 
 */
StyleTableView::StyleTableView(QWidget* parent): QTableView(parent) {}
QModelIndex StyleTableView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers) {
    QModelIndex current = currentIndex();
    int row = current.row();
    int col = current.column();
    if(_plot_adapter) {
        int lastRow = _style_database_adapter.style_items_count(_plot_adapter->type()) - 1;
        int lastCol = 1;
        //tab press on the last cell
        if(cursorAction == MoveNext && row == lastRow && col == lastCol)
            emit lastItemTab();
        //tab+shift (backtab) press on the first cell
        if(cursorAction == MovePrevious && row == 0 && col == 0)
            emit firstItemBackTab();
    }
    return QTableView::moveCursor(cursorAction, modifiers);
}
void StyleTableView::set_plot_adapter(PlotAdapter* adapter) {
    _plot_adapter = adapter;
}
StyleDatabaseAdapter* StyleTableView::style_database_adapter() { return &_style_database_adapter; }
/**
 * StyleDelegate
 * -------------
 * Gestion des editeurs d'éléments de style de la table
 */
StyleDelegate::StyleDelegate(StyleDatabaseAdapter* style_database_adapter, QObject* parent) : 
    QStyledItemDelegate(parent), 
    _plot_adapter(nullptr),
    _style_database_adapter(style_database_adapter) {}
void StyleDelegate::set_plot_adapter(PlotAdapter* plot_adapter) { _plot_adapter = plot_adapter; }
QWidget* StyleDelegate::createEditor(QWidget* parent,
                                     const QStyleOptionViewItem& option,
                                     const QModelIndex& index) const {
    Q_UNUSED(option)
    if(!_plot_adapter) return nullptr;
    if (index.column() != 1) return nullptr;
    QLineEdit* le;
    QComboBox* cbb;
    QSpinBox* spn;
    QDoubleSpinBox* dspn;
    QColorDialog* cdlg;
    QList<QVariant> args;
    int row = index.row();
    PlotType plot_type = _plot_adapter->type();
    StyleItemType item_type = _style_database_adapter->style_item_type(plot_type, row);
    switch (item_type) {
    case StyleItemType::String:
        le = new QLineEdit(parent);
        return le;
    case StyleItemType::Integer:
        le = new QLineEdit(parent);
        le->setValidator(new QIntValidator(parent));
        return le;
    case StyleItemType::Double:
        le = new QLineEdit(parent);
        le->setValidator(new QDoubleValidator(parent));
        return le;
    case StyleItemType::CustomIcon:
        cbb = new QComboBox(parent);
        _style_database_adapter->populate_icons_combo(plot_type, row, cbb);
        return cbb;
    case StyleItemType::Integer_Range:
        spn = new QSpinBox(parent);
        args = _style_database_adapter->style_item_args(plot_type, row);
        spn->setRange(args[0].toInt(), args[1].toInt());
        return spn;
    case StyleItemType::Double_Range:
        dspn = new QDoubleSpinBox(parent);
        args = _style_database_adapter->style_item_args(plot_type, row);
        dspn->setRange(args[0].toDouble(), args[1].toDouble());
        dspn->setSingleStep(args[2].toDouble());
        return dspn;
    case StyleItemType::Color:
        cdlg = new QColorDialog(parent);
        cdlg->setOption(QColorDialog::ShowAlphaChannel, true);
        return cdlg;
    default:
        return nullptr;
    }
    return nullptr;
}
void StyleDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    if (!_plot_adapter) return;
    if (index.column() != 1) return;
    int row = index.row();
    PlotType plot_type = _plot_adapter->type();
    StyleItemType item_type = _style_database_adapter->style_item_type(plot_type, row);
    switch (item_type) {
    case StyleItemType::String:
        if (auto* le = qobject_cast<QLineEdit*>(editor))
            le->setText(_style_database_adapter->style_item_value(plot_type, row).toString());
        break;
    case StyleItemType::Integer:
        if (auto* le = qobject_cast<QLineEdit*>(editor))
            le->setText(QString("%1").arg(_style_database_adapter->style_item_value(plot_type, row).toInt()));
        break;
    case StyleItemType::Double:
        if (auto* le = qobject_cast<QLineEdit*>(editor))
            le->setText(QString("%1").arg(_style_database_adapter->style_item_value(plot_type, row).toDouble()));
        break;
    case StyleItemType::CustomIcon:
        if (auto* cbb = qobject_cast<QComboBox*>(editor))
            cbb->setCurrentIndex(_style_database_adapter->style_item_value(plot_type, row).toInt());
        break;
    case StyleItemType::Integer_Range:
        if (auto* spn = qobject_cast<QSpinBox*>(editor)) {
            spn->setValue(_style_database_adapter->style_item_value(plot_type, row).toInt());
        }
        break;
    case StyleItemType::Double_Range:
        if (auto* dspn = qobject_cast<QDoubleSpinBox*>(editor))
            dspn->setValue(_style_database_adapter->style_item_value(plot_type, row).toDouble());
        break;
    case StyleItemType::Color:
        if (auto* cdlg = qobject_cast<QColorDialog*>(editor)) {
            const QColor color(_style_database_adapter->style_item_value(plot_type, row).toString());
            if(color.isValid())
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
    if (index.column() == 1) {
        int row = index.row();
        PlotType plot_type = _plot_adapter->type();
        StyleItemType item_type = _style_database_adapter->style_item_type(plot_type, row);
        if (auto* le = qobject_cast<QLineEdit*>(editor)) {
            switch(item_type) {
            case StyleItemType::String:
                new_value = le->text();
                break;
            case StyleItemType::Integer:
                new_value = le->text().toInt();
                break;
            case StyleItemType::Double:
                new_value = le->text().toDouble();
                break;
            default:
                break;
            }
        }
        else if (auto* cbb = qobject_cast<QComboBox*>(editor))
            new_value = cbb->currentIndex();
        else if (auto* spn = qobject_cast<QSpinBox*>(editor))
            new_value = spn->value();
        else if (auto* dspn = qobject_cast<QDoubleSpinBox*>(editor))
            new_value = dspn->value();
        else if (auto* cdlg = qobject_cast<QColorDialog*>(editor))
            new_value = cdlg->currentColor();
    }
    model->setData(index, new_value, Qt::EditRole);
}
void StyleDelegate::updateEditorGeometry(QWidget* editor,
                                         const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const {
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}
/** 
 * StyleTableModel
 * --------------
 * Modele dedié à l'affichage/définition des styles
 */
StyleTableModel::StyleTableModel(StyleDatabaseAdapter* style_database_adapter, QObject* parent): 
    QStandardItemModel(parent), 
    _plot_adapter(nullptr),
    _style_database_adapter(style_database_adapter) {
    setColumnCount(2);
    setHorizontalHeaderLabels({ QString(), "Value" });
}
Qt::ItemFlags StyleTableModel::flags(const QModelIndex & index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;
    Qt::ItemFlags flags = QStandardItemModel::flags(index);
    if(index.column() == 1) {
        if(_style_database_adapter->style_item_type(_plot_adapter->type(), index.row()) == StyleItemType::Bool)
            flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
        else
            flags |= Qt::ItemIsEditable;
    }
    return flags;
}
void StyleTableModel::set_plot_adapter(PlotAdapter* plot_adapter) {
    _plot_adapter = plot_adapter;
    setRowCount(_plot_adapter ? _style_database_adapter->style_items_count(_plot_adapter->type()): 0);
    update_model();
}
QVariant StyleTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || !_plot_adapter) return {};
    QVariant value = {};
    int row = index.row();
    int column = index.column();
    PlotType plot_type = _plot_adapter->type();
    StyleItemType type = _style_database_adapter->style_item_type(plot_type, row);
    if(role == Qt::DisplayRole) {
        if(column == 0) {
            QString label = _style_database_adapter->style_item_label(plot_type, row);
            return label;
        }
        else if(column == 1) {
            value = _style_database_adapter->style_item_value(plot_type, row);
            switch(type) {
            case StyleItemType::Integer:
            case StyleItemType::Integer_Range:
                return value.toInt();
            case StyleItemType::Double:
            case StyleItemType::Double_Range:
                return value.toDouble();
            case StyleItemType::String:
                return value.toString();
            default:
                break;
            }
        }
    }
    else if(role == Qt::DecorationRole) {
        QString path;
        if(column == 1) {
            switch(type) {
            case StyleItemType::CustomIcon:
                path = _style_database_adapter->style_item_icon_path(plot_type, row);
                return IhmTools::load_colored_icon(path);
            case StyleItemType::Color:
                value = _style_database_adapter->style_item_value(plot_type, row);
                return QColor(value.toString());
            default:
                break;
            }
        }
    }
    else if(role == Qt::CheckStateRole) {
        if(column == 1 && type == StyleItemType::Bool) {
            return _style_database_adapter->style_item_value(plot_type, row).toBool() ? Qt::Checked : Qt::Unchecked;
        }
    }
    return {};
}
bool StyleTableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid() || !_plot_adapter) return {};
    int row = index.row();
    int column = index.column();
    PlotType plot_type = _plot_adapter->type();
    if(column == 1) {
        if(role == Qt::CheckStateRole)
            _style_database_adapter->set_style_item_value(plot_type, row, value.toInt() == Qt::Checked ? true: false);
        else if(role == Qt::EditRole)
            _style_database_adapter->set_style_item_value(plot_type, row, value);
        return true;
    }
    return QStandardItemModel::setData(index, value, role);
}
void StyleTableModel::update_model() {
    beginResetModel();
    endResetModel();
}