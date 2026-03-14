#include "chip_widget.h"

ChipWidget::ChipWidget(const QString &text, const QIcon &icon, QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(8, 4, 8, 4);
    layout->setSpacing(4);

    m_iconLabel = new QLabel(this);
    if (!icon.isNull())
        m_iconLabel->setPixmap(icon.pixmap(16, 16));

    m_textLabel = new QLabel(text, this);
    m_textLabel->setStyleSheet("QLabel { color : black; }");

    m_closeButton = new QPushButton("×", this);
    m_closeButton->setFixedSize(16, 16);
    m_closeButton->setFlat(true);
    m_closeButton->setStyleSheet("QPushButton { border: none; color: black; }");

    layout->addWidget(m_iconLabel);
    layout->addWidget(m_textLabel);
    layout->addWidget(m_closeButton);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // setStyleSheet("background-color: #d0d0d0; border-radius: 8px;");

    connect(m_closeButton, &QPushButton::clicked, this, [this]() {
        emit chip_closed(this);
    });
}
void ChipWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setWidth(rect.width()-1);
    rect.setHeight(rect.height()-1);
    painter.drawRoundedRect(rect, 10, 10);
}