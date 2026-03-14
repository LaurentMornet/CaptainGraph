#pragma once
#include <QVariantMap>
#include <QColor>
#include "scatter/scatter_style.h"

class StyleMapper {
public:
    static QVariantMap to_map(const BaseStyle* base);
    static std::unique_ptr<BaseStyle> from_map(const QString& type, const QVariantMap& map);
};
