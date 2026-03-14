#include "bar_style_frame.h"

BarStyleFrame::BarStyleFrame(QWidget * parent) : QFrame(parent), IStyleEditor() {
    auto layout = new QFormLayout(this);
    _btn_color = new QPushButton("Couleur remplissage");
    _spn_border = new QDoubleSpinBox(this);
    _spn_border->setRange(0, 10);

    layout->addRow("Couleur :", _btn_color);
    layout->addRow("Epaisseur bord :", _spn_border);

    connect(_btn_color, &QPushButton::clicked, this, [this] {
        QColor c = QColorDialog::getColor(Qt::green, this, "Choisir une couleur");
        if (c.isValid()) _chosen_color = c;
    });
}
void BarStyleFrame::set_style(const BaseStyle * base) {
    const BarStyle * s = dynamic_cast<const BarStyle*>(base);
    if (!s) return;
    _chosen_color = QColor(QString(s->fill_color.c_str()));
    _spn_border->setValue(static_cast<int>(s->border_width));
}
std::unique_ptr<BaseStyle> BarStyleFrame::get_style() const {
    auto s = std::make_unique<BarStyle>();
    s->fill_color = _chosen_color.name().toStdString();
    s->border_width = _spn_border->value();
    return s;
}