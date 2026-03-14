#pragma once
#include <QFrame>
#include <QFormLayout>
#include <QVariant>
#include <QMap>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QColorDialog>
#include "style_field_desc.h"

// generic_style_frame.h
#pragma once
#include <QFrame>
#include <QFormLayout>
#include <QVariant>
#include <QMap>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QColorDialog>
#include "style_field_desc.h"

class GenericStyleFrame : public QFrame {
    Q_OBJECT
public:
    explicit GenericStyleFrame(QWidget* parent = nullptr);
    void set_fields(const QList<StyleFieldDesc>& fieldList);
    QVariantMap values() const;
    void set_values(const QVariantMap& values);
private:
    QFormLayout* layout;
    QList<StyleFieldDesc> fields;
    QMap<QString, QWidget*> editors;
    QWidget* create_editor(const StyleFieldDesc& f);
    QVariant read_value(const QString& key, FieldType type) const;
    void set_values(const QString& key, FieldType type, const QVariant& val);
    void clear();
};
