#pragma once

#include <QColor>
#include <QList>
#include <QMap>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QComboBox>

#include <variant>

#include "../common/plot.h"
#include "../ihm/ihm_tools.h"

class StyleDatabaseAdapter: public QObject {
    Q_OBJECT
private:
    QMap<PlotType, QMap<QString, QVariant>> _styledef;
public:
    explicit StyleDatabaseAdapter();
    unsigned int style_items_count(const PlotType& type) const;
    unsigned int style_items_count(const int& type) const;

    StyleItemType style_item_type(const PlotType& type, const int& idx) const;
    StyleItemType style_item_type(const int& type, const int& idx) const;

    QString style_item_label(const PlotType& type, const int& idx) const;
    QString style_item_label(const int& type, const int& idx) const;

    QVariant style_item_value(const PlotType& type, const int& idx) const;
    QVariant style_item_value(const int& type, const int& idx) const;

    void set_style_item_value(const PlotType& type, const int& idx, const QVariant& value);
    void set_style_item_value(const int& type, const int& idx, const QVariant& value);

    QList<QVariant> style_item_args(const PlotType& type, const int& idx) const;
    QList<QVariant> style_item_args(const int& type, const int& idx) const;

    QString style_item_icon_path(const PlotType& type, const int& idx) const;

    void populate_icons_combo(const PlotType& type, const int& idx, QComboBox* cbb) const;

signals:
    void styledef_setted(QString, QVariant);
};