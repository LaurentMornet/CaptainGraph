#include "style_adapter.h"

StyleDatabaseAdapter::StyleDatabaseAdapter() {
    for(unsigned int i=0; i < Plot::__static__plots_types_indexes__.size(); i++)
        _styledef[Plot::__static__plots_types_indexes__[i]] = {};
}
unsigned int StyleDatabaseAdapter::style_items_count(const PlotType& type) const {
    return Plot::__static__style_database__[type].size();
}
unsigned int StyleDatabaseAdapter::style_items_count(const int& type) const {
    return style_items_count(Plot::__static__plots_types_indexes__[type]);
}
StyleItemType StyleDatabaseAdapter::style_item_type(const PlotType& type, const int& idx) const {
    return Plot::__static__style_database__[type][idx].type();
}
StyleItemType StyleDatabaseAdapter::style_item_type(const int& type, const int& idx) const {
    return style_item_type(Plot::__static__plots_types_indexes__[type], idx);
}
QString StyleDatabaseAdapter::style_item_label(const PlotType& type, const int& idx) const {
    return QString::fromStdString(Plot::__static__style_database__[type][idx].label());
}
QString StyleDatabaseAdapter::style_item_label(const int& type, const int& idx) const {
    return style_item_label(Plot::__static__plots_types_indexes__[type], idx);
}
QVariant StyleDatabaseAdapter::style_item_value(const PlotType& type, const int& idx) const {
    QString id = QString::fromStdString(Plot::__static__style_database__[type][idx].id());
    StyleItemType style_item_type = Plot::__static__style_database__[type][idx].type();
    if(_styledef[type].contains(id))
        return _styledef[type][id];
    StyleItemValue value = Plot::__static__style_database__[type][idx].initial(); 
    switch(style_item_type) {
    case StyleItemType::Bool:
        return QVariant(std::get<bool>(value));
    case StyleItemType::Integer:
    case StyleItemType::Integer_Range:
    case StyleItemType::CustomIcon:
        return QVariant(std::get<int>(value));
    case StyleItemType::String:
        return QVariant(QString::fromStdString(std::get<std::string>(value)));
    case StyleItemType::Double:
    case StyleItemType::Double_Range:
        return QVariant(std::get<double>(value));
    case StyleItemType::Color:
        return QVariant(QColor(QString::fromStdString(std::get<std::string>(value))));
    }
    return {};
}
QVariant StyleDatabaseAdapter::style_item_value(const int& type, const int& idx) const {
    return style_item_value(Plot::__static__plots_types_indexes__[type], idx);
}
void StyleDatabaseAdapter::set_style_item_value(const PlotType& type, const int& idx, const QVariant& value) {
    QString id = QString::fromStdString(Plot::__static__style_database__[type][idx].id());
    _styledef[type][id] = value;
    emit styledef_setted(id, value);
}
void StyleDatabaseAdapter::set_style_item_value(const int& type, const int& idx, const QVariant& value) {
    set_style_item_value(Plot::__static__plots_types_indexes__[type], idx, value);
}
QList<QVariant> StyleDatabaseAdapter::style_item_args(const PlotType& type, const int& idx) const {
    QList<QVariant> list;
    for(unsigned int i=0; i<Plot::__static__style_database__[type][idx].args_count(); i++) {
        StyleItemType style_item_type = Plot::__static__style_database__[type][idx].type();
        switch(style_item_type) {
        case StyleItemType::Bool:
            list.append(QVariant(std::get<bool>(Plot::__static__style_database__[type][idx].arg(i)))); break;
        case StyleItemType::Integer:
        case StyleItemType::Integer_Range:
        case StyleItemType::CustomIcon:
            list.append(QVariant(std::get<int>(Plot::__static__style_database__[type][idx].arg(i)))); break;
        case StyleItemType::String:
            list.append(QVariant(QString::fromStdString(std::get<std::string>(Plot::__static__style_database__[type][idx].arg(i))))); break;
        case StyleItemType::Double:
        case StyleItemType::Double_Range:
            list.append(QVariant(std::get<double>(Plot::__static__style_database__[type][idx].arg(i)))); break;
        case StyleItemType::Color:
            list.append(QVariant(QColor(QString::fromStdString(std::get<std::string>(Plot::__static__style_database__[type][idx].arg(i)))))); break;
        }
    }
    return list;
}
QList<QVariant> StyleDatabaseAdapter::style_item_args(const int& type, const int& idx) const {
    return style_item_args(Plot::__static__plots_types_indexes__[type], idx);
}