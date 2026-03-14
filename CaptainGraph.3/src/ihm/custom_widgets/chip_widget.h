#pragma once

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

class ChipWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChipWidget(const QString &text, const QIcon &icon = QIcon(), QWidget *parent = nullptr);

signals:
    void chip_closed(ChipWidget *chip);

private:
    QLabel *m_iconLabel;
    QLabel *m_textLabel;
    QPushButton *m_closeButton;
    virtual void paintEvent(QPaintEvent* event) override;
};
