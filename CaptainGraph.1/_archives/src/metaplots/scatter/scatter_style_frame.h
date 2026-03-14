#pragma once

#include <QFrame>
#include <QColorDialog>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QFormLayout>
#include "../base_style.h"
#include "../istyle_editor.h"
#include "scatter_style.h"

class ScatterStyleFrame: public QFrame, public IStyleEditor {
    Q_OBJECT
    Q_INTERFACES(IStyleEditor)
private:
    QPushButton *   _btn_color;
    QSpinBox *      _spn_marker_size;
    QCheckBox *     _chk_legend;
    QColor          _chosen_color = Qt::blue;
public:
    explicit ScatterStyleFrame(QWidget * parent=nullptr);
    virtual void set_style(const BaseStyle * base) override;
    virtual std::unique_ptr<BaseStyle> get_style() const override;
};