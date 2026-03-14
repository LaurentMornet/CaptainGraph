#include "chips_container.h"
#include <QTimer>

ChipsContainer::ChipsContainer(QWidget *parent)
    : QWidget(parent)
{
    m_layout = new FlowLayout;
    setLayout(m_layout);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
}

void ChipsContainer::add_chip(const QString &text, const QIcon &icon)
{
    auto *chip = new ChipWidget(text, icon);
    connect(chip, &ChipWidget::chip_closed, this, &ChipsContainer::remove_chip);
    m_layout->addWidget(chip);
    updateGeometry();
    if (parentWidget()) parentWidget()->updateGeometry();
}

void ChipsContainer::remove_chip(ChipWidget *chip)
{
    m_layout->removeWidget(chip);
    chip->deleteLater();
    updateGeometry();
    if (parentWidget()) parentWidget()->updateGeometry();
}
QSize ChipsContainer::sizeHint() const
{
    int availableWidth = width();
    if (availableWidth <= 0) {
        if (parentWidget())
            availableWidth = parentWidget()->width();
        if (availableWidth <= 0)
            availableWidth = 200;
    }

    int h = m_layout->heightForWidth(availableWidth);
    QMargins m = m_layout->contentsMargins();
    h += m.top() + m.bottom();

    // 🔹 Laisse Qt choisir la largeur, ne la bloque pas
    return QSize(sizePolicy().horizontalPolicy() == QSizePolicy::Expanding ? 0 : availableWidth, h);
}


QSize ChipsContainer::minimumSizeHint() const
{
    // similar to sizeHint for our use-case
    return sizeHint();
}

void ChipsContainer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Force layout system to re-evaluate preferred/minimum sizes
    updateGeometry();

    // also invalidate layouts up the chain so parents recalc their geometry
    QWidget *p = parentWidget();
    while (p) {
        if (p->layout()) p->layout()->invalidate();
        p->updateGeometry();
        p = p->parentWidget();
    }
}
// void ChipsContainer::showEvent(QShowEvent* event) {
//     updateGeometry();
// }