#include "generic_style_frame.h"

GenericStyleFrame::GenericStyleFrame(QWidget* parent)
    : QFrame(parent), layout(new QFormLayout(this)) {}

void GenericStyleFrame::set_fields(const QList<StyleFieldDesc>& fieldList) {
    clear();
    fields = fieldList;
    for (const auto& f : fields) {
        QWidget* editor = create_editor(f);
        layout->addRow(f.label + " :", editor);
        editors[f.key] = editor;
    }
}

QVariantMap GenericStyleFrame::values() const {
    QVariantMap result;
    for (const auto& f : fields)
        result[f.key] = read_value(f.key, f.type);
    return result;
}

void GenericStyleFrame::set_values(const QVariantMap& values) {
    for (const auto& f : fields)
        if (values.contains(f.key))
            set_values(f.key, f.type, values[f.key]);
}

QWidget* GenericStyleFrame::create_editor(const StyleFieldDesc& f) {
    switch (f.type) {
    case FieldType::Int: {
        auto* spin = new QSpinBox;
        spin->setValue(f.default_value.toInt());
        return spin;
    }
    case FieldType::Double: {
        auto* spin = new QDoubleSpinBox;
        spin->setValue(f.default_value.toDouble());
        return spin;
    }
    case FieldType::Bool: {
        auto* check = new QCheckBox;
        check->setChecked(f.default_value.toBool());
        return check;
    }
    case FieldType::Enum: {
        auto* combo = new QComboBox;
        combo->addItems(f.enum_values);
        combo->setCurrentText(f.default_value.toString());
        return combo;
    }
    case FieldType::Color: {
        auto* btn = new QPushButton("Choisir couleur");
        QColor default_color(f.default_value.toString());
        QObject::connect(btn, &QPushButton::clicked, [btn, default_color]() mutable {
            QColor c = QColorDialog::getColor(default_color, btn, "Couleur");
            if (c.isValid()) btn->setProperty("chosenColor", c);
        });
        btn->setProperty("chosenColor", default_color);
        return btn;
    }}
    return nullptr;
}

QVariant GenericStyleFrame::read_value(const QString& key, FieldType type) const {
    QWidget* w = editors.value(key);
    if (!w) return {};

    switch (type) {
    case FieldType::Int: return qobject_cast<QSpinBox*>(w)->value();
    case FieldType::Double: return qobject_cast<QDoubleSpinBox*>(w)->value();
    case FieldType::Bool: return qobject_cast<QCheckBox*>(w)->isChecked();
    case FieldType::Enum: return qobject_cast<QComboBox*>(w)->currentText();
    case FieldType::Color: return w->property("chosen_color").value<QColor>().name();
    }
    return {};
}

void GenericStyleFrame::set_values(const QString& key, FieldType type, const QVariant& val) {
    QWidget* w = editors.value(key);
    if (!w) return;

    switch (type) {
    case FieldType::Int: qobject_cast<QSpinBox*>(w)->setValue(val.toInt()); break;
    case FieldType::Double: qobject_cast<QDoubleSpinBox*>(w)->setValue(val.toDouble()); break;
    case FieldType::Bool: qobject_cast<QCheckBox*>(w)->setChecked(val.toBool()); break;
    case FieldType::Enum: qobject_cast<QComboBox*>(w)->setCurrentText(val.toString()); break;
    case FieldType::Color: w->setProperty("chosen_color", QColor(val.toString())); break;
    }
}

void GenericStyleFrame::clear() {
    QLayoutItem* item;
    while ((item = layout->takeAt(0))) {
        delete item->widget();
        delete item;
    }
    editors.clear();
}