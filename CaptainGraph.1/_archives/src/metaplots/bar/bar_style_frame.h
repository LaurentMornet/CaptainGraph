#pragma once

#include <QFrame>
#include <QColorDialog>
#include <QSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include "../base_style.h"
#include "../istyle_editor.h"
#include "bar_style.h"

class BarStyleFrame: public QFrame, public IStyleEditor {
    Q_OBJECT
    Q_INTERFACES(IStyleEditor)
private:
    QPushButton *       _btn_color;
    QDoubleSpinBox *    _spn_border;
    QColor              _chosen_color = Qt::green;
public:
    explicit BarStyleFrame(QWidget * parent=nullptr);
    virtual void set_style(const BaseStyle * base) override;
    virtual std::unique_ptr<BaseStyle> get_style() const override;
};