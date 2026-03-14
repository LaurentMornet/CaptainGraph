#pragma once

#include <QComboBox>
#include <QIcon>
#include <QStyleFactory>

class LineStyleWidget: public QComboBox {
private:
private slots:
    void _cb_selection_changed();
public:
    explicit LineStyleWidget(QWidget * parent=nullptr);
    virtual ~LineStyleWidget() = default;
};