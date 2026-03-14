#pragma once

#include <QWidget>
#include <QResizeEvent>
// #include <QShowEvent>
#include "flow_layout.h"
#include "chip_widget.h"

class ChipsContainer : public QWidget {
    Q_OBJECT
public:
    explicit ChipsContainer(QWidget *parent = nullptr);
    void add_chip(const QString &text, const QIcon &icon = QIcon());
    void clear();
    
    // overrides
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    // virtual void showEvent(QShowEvent* event) override;

private slots:
    void remove_chip(ChipWidget *chip);

private:
    FlowLayout *m_layout;
    // virtual void resizeEvent(QResizeEvent *event) override;
};
