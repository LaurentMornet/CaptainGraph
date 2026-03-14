#pragma once

#include <QComboBox>
#include <QFormLayout>
#include <QFrame>
// #include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include "main_window.h"
#include "plot_style.h"

class PlotFrame: public QFrame {
    Q_OBJECT
private:
    QSize sizeHint() const override;
    MainWindow* _main_window;
    std::unique_ptr<PlotAdapter> _plot_adapter;
    QFrame* _main_frame;
    QFrame* _header_frame;
    QList<QLineEdit*> _edt_feature;
    QFrame* _features_frame;
    QFormLayout* _features_layout;
    QComboBox* _cbb_type;
    QLineEdit* _edt_label;
    StyleTableModel* _style_table_model;
    StyleDelegate* _style_delegate;
    StyleTableView* _style_table;
    bool _freeze=false;
    void _update_features();
public:
    PlotFrame(MainWindow* parent);
    virtual ~PlotFrame()=default;
    PlotFrame(const PlotFrame&)=delete;
    PlotFrame& operator=(const PlotFrame&)=delete;
    void set_plot_adapter(std::unique_ptr<PlotAdapter> plot_adapter);
private slots:
    void _cb_label_changed();
    void _cb_type_changed();
    void _cb_feature_changed(unsigned int idx);
};