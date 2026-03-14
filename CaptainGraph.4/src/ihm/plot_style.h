#pragma once

#include <QTableView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QColorDialog>
#include <QCheckBox>

#include "../adapter/plot_adapter.h"
#include "../adapter/style_adapter.h"
class StyleTableView: public QTableView {
    Q_OBJECT
private:
    PlotAdapter* _plot_adapter;
    StyleDatabaseAdapter _style_database_adapter;
protected:
    virtual QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
public:
    explicit StyleTableView(QWidget* parent=nullptr);
    void set_plot_adapter(PlotAdapter* adapter);
    StyleDatabaseAdapter* style_database_adapter();
signals:
    void firstItemBackTab();
    void lastItemTab();
};

class StyleDelegate : public QStyledItemDelegate {
    Q_OBJECT
private:
    PlotAdapter* _plot_adapter;
    StyleDatabaseAdapter* _style_database_adapter;
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                              const QModelIndex& index) const override;
public:
    explicit StyleDelegate(StyleDatabaseAdapter* style_database_adapter, QObject* parent = nullptr);
    void set_plot_adapter(PlotAdapter* plot_adapter);
};

class StyleTableModel: public QStandardItemModel {
    Q_OBJECT
private:
    PlotAdapter* _plot_adapter;
    StyleDatabaseAdapter* _style_database_adapter;
    virtual Qt::ItemFlags flags(const QModelIndex & index) const override;
public:
    explicit StyleTableModel(StyleDatabaseAdapter* style_database_adapter, QObject* parent=nullptr);
    void set_plot_adapter(PlotAdapter* adapter);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    void update_model();
};