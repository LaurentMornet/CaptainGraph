#include "scatter_style_frame.h"

ScatterStyleFrame::ScatterStyleFrame(QWidget * parent): QFrame(parent), IStyleEditor() {
    auto layout = new QFormLayout(this);
    _btn_color = new QPushButton("Couleur ligne");
    _spn_marker_size = new QSpinBox(this);
    _spn_marker_size->setRange(1, 20);
    _chk_legend = new QCheckBox("Afficher légende");

    layout->addRow("Couleur :", _btn_color);
    layout->addRow("Taille marqueur :", _spn_marker_size);
    layout->addRow("", _chk_legend);

    connect(_btn_color, &QPushButton::clicked, this, [this] {
        QColor c = QColorDialog::getColor(Qt::blue, this, "Choisir une couleur");
        if (c.isValid()) _chosen_color = c;
    });
}
void ScatterStyleFrame::set_style(const BaseStyle * base) {
    const ScatterStyle * s = dynamic_cast<const ScatterStyle*>(base);
    if (!s) return;
    _chosen_color = QColor(QString(s->line_color.c_str()));
    _spn_marker_size->setValue(static_cast<int>(s->marker_size));
    _chk_legend->setChecked(s->show_legend);
}
std::unique_ptr<BaseStyle> ScatterStyleFrame::get_style() const {
    auto s = std::make_unique<ScatterStyle>();
    s->line_color = _chosen_color.name().toStdString();
    s->marker_size = _spn_marker_size->value();
    s->show_legend = _chk_legend->isChecked();
    return s;
}