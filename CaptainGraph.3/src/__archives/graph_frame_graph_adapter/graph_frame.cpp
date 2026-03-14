#include "graph_frame.h"
#include <QDebug>
#include <QToolBar>

/* ----------------------------- GraphFrame ----------------------------- */

GraphFrame* GraphFrame::__instance = nullptr;

GraphFrame& GraphFrame::get_instance(QWidget* parent) {
    if (!__instance) {
        if (!parent) {
            qCritical() << "GraphFrame::get_instance() appelé sans parent lors de la première création !";
            throw std::runtime_error("GraphFrame doit être créé avec un parent lors du premier appel.");
        }
        __instance = new GraphFrame(parent);
        QObject::connect(__instance, &QObject::destroyed, []() { __instance = nullptr; });
    }
    return *__instance;
}

GraphFrame::GraphFrame(QWidget* parent)
    : QFrame(parent), _graph_adapter(nullptr) {
    auto* main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(main_layout);

    _main_frame = new QFrame(this);
    main_layout->addWidget(_main_frame);

    auto* layout = new QVBoxLayout(_main_frame);
    _data_table = new QTableView(_main_frame);

    // On crée le modèle et le delegate
    _data_model = new GraphDataModel(_graph_adapter);
    _data_table->setModel(_data_model);
    _data_table->setItemDelegate(new GraphDataDelegate(_data_model));

    _data_table->verticalHeader()->hide();
    _data_table->horizontalHeader()->setStretchLastSection(true);
    _data_table->horizontalHeader()->resizeSection(0, 150);
    layout->addWidget(_data_table);

    // Gestion des plots
    auto* toolbar = new QToolBar(this);
    auto* action = toolbar->addAction("New plot");
    layout->addWidget(toolbar);

    auto* list_view = new QListView(this);
    layout->addWidget(list_view);

    _main_frame->setVisible(false);
}

GraphFrame::~GraphFrame() {
    _graph_adapter = nullptr;
}

void GraphFrame::set_graph_adapter(GraphAdapter* graph_adapter) {
    if (graph_adapter == _graph_adapter)
        return;
    _graph_adapter = graph_adapter;
    _data_model->set_adapter(_graph_adapter);
    bool visible = _graph_adapter && _graph_adapter->graph();
    _main_frame->setVisible(visible);
    if (visible)
        _data_model->update_model();
}

QSize GraphFrame::sizeHint() const {
    return QSize(300, 300);
}

/* ----------------------------- GraphDataModel ----------------------------- */

GraphDataModel::GraphDataModel(GraphAdapter* adapter)
    : QStandardItemModel(adapter), _adapter(adapter) {
    setColumnCount(2);
    setRowCount(_adapter ? _adapter->row_count() : 0);
    setHorizontalHeaderLabels({ QString(), "Value" });
}

void GraphDataModel::set_adapter(GraphAdapter* adapter) {
    _adapter = adapter;
    setRowCount(_adapter ? _adapter->row_count() : 0);
    update_model();
}

GraphAdapter* GraphDataModel::adapter() {
    return _adapter.data();
}

Qt::ItemFlags GraphDataModel::flags(const QModelIndex& index) const {
    if (!index.isValid() || !_adapter) return Qt::NoItemFlags;
    Qt::ItemFlags f = QStandardItemModel::flags(index);
    if (index.column() == 1) {
        if (_adapter->is_editable_row(index.row())) f |= Qt::ItemIsEditable;
        if (_adapter->is_checkable_row(index.row())) f |= Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    }
    return f;
}

QVariant GraphDataModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || !_adapter) return {};
    if (role == Qt::DisplayRole) {
        return index.column() == 0 ? _adapter->label_for_row(index.row())
                                    : (_adapter->is_editable_row(index.row()) ? _adapter->value_for_row(index.row()) : QVariant());
    }
    if (role == Qt::CheckStateRole && _adapter->is_checkable_row(index.row()) && index.column() == 1) {
        return _adapter->value_for_row(index.row()).toBool() ? Qt::Checked : Qt::Unchecked;
    }
    return {};
}

bool GraphDataModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid() || !_adapter || index.column() != 1) return false;

    if (role == Qt::EditRole && _adapter->is_editable_row(index.row())) {
        _adapter->set_value_for_row(index.row(), value);
    } else if (role == Qt::CheckStateRole && _adapter->is_checkable_row(index.row())) {
        _adapter->set_value_for_row(index.row(), value == Qt::Checked);
    } else {
        return false;
    }

    emit dataChanged(index, index);
    return true;
}

void GraphDataModel::update_model() {
    beginResetModel();
    endResetModel();
}

/* ----------------------------- GraphDataDelegate ----------------------------- */

GraphDataDelegate::GraphDataDelegate(GraphDataModel* model) : QStyledItemDelegate(model), _model(model) {}

QWidget* GraphDataDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& index) const {
    if (!_model || index.column() != 1) return nullptr;
    if (_model->adapter()->is_editable_row(index.row()))
        return new QLineEdit(parent);
    return nullptr;
}

void GraphDataDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    if (!_model || index.column() != 1) return;
    if (auto* le = qobject_cast<QLineEdit*>(editor))
        le->setText(_model->adapter()->value_for_row(index.row()).toString());
}

void GraphDataDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    if (!_model || index.column() != 1) return;
    if (auto* le = qobject_cast<QLineEdit*>(editor))
        model->setData(index, le->text(), Qt::EditRole);
}

void GraphDataDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex&) const {
    editor->setGeometry(option.rect);
}