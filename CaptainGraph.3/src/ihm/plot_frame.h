#pragma once

#include <QComboBox>
#include <QFormLayout>
#include <QFrame>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTableView>

#include "main_window.h"
#include "../adapter/plot_adapter.h"
#include "../adapter/style_adapter.h"

class StyleTableModel: public QStandardItemModel {
    Q_OBJECT
private:
    PlotAdapter* _plot_adapter;
    StyleDatabaseAdapter _style_database_adapter;
public:
    explicit StyleTableModel(QObject* parent=nullptr);
    void set_plot_adapter(PlotAdapter* adapter);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    void update_model();
};

class PlotFrame: public QFrame {
    Q_OBJECT
private:
    QSize sizeHint() const override;
    MainWindow* _main_window;
    PlotAdapter* _plot_adapter;
    QFrame* _header_frame;
    QList<QLineEdit*> _edt_feature;
    QFrame* _features_frame;
    QFormLayout* _features_layout;
    QComboBox* _cbb_type;
    QLineEdit* _edt_label;
    StyleTableModel* _style_table_model;
    QTableView* _style_table;
    void _update_features();
public:
    PlotFrame(MainWindow* parent);
    virtual ~PlotFrame()=default;
    PlotFrame(const PlotFrame&)=delete;
    PlotFrame& operator=(const PlotFrame&)=delete;
    void set_plot_adapter(PlotAdapter* plot_adapter);
private slots:
    void _cb_label_changed();
    void _cb_type_changed();
    void _cb_feature_changed(unsigned int idx);
};