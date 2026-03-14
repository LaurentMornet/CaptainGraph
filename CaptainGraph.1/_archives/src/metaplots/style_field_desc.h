#pragma once
#include <QString>
#include <QStringList>
#include <QVariant>

enum class FieldType {
    Int,
    Double,
    Bool,
    Color,
    Enum
};

struct StyleFieldDesc {
    QString key;
    QString label;
    FieldType type;
    QVariant default_value;
    QStringList enum_values;
};