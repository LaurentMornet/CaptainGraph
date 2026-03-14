#include "plot_adapter.h"
#include <iostream>

PlotAdapter::PlotAdapter(Plot* plot, QObject* parent): QObject(parent), _plot(plot) {}
Plot* PlotAdapter::plot() const { return _plot; }
/**
 * Inteface simple
 */
PlotType PlotAdapter::type() const { return _plot->type(); }
int PlotAdapter::type_as_int() const {
    for(unsigned int i = 0; i < Plot::__static__plots_types_indexes__.size(); i++)
        if(Plot::__static__plots_types_indexes__[i] == _plot->type()) return i;
    return -1;
}
QString PlotAdapter::label() const { return QString::fromStdString(_plot->label()); }
void PlotAdapter::set_type(const PlotType& type) { _plot->set_type(type); }
void PlotAdapter::set_type(const unsigned int& index) {
    _plot->set_type(Plot::__static__plots_types_indexes__[index]);
}
void PlotAdapter::set_label(const QString& label) { 
    _plot->set_label(label.toStdString()); 
    emit label_changed(label);
}
/**
 * Gestion des plots
 */
/*static*/ QMap<PlotType, QString> PlotAdapter::plots_types_map() {
    QMap<PlotType, QString> map;
    for(auto [key, item]: Plot::__static__plots_types_map__) {
        map[key] = QString::fromStdString(item);
    }
    return map;
}
QList<QString> PlotAdapter::style_keys() {
    QList<QString> list;
    for(auto& item: _plot->style_keys())
        list.append(QString::fromStdString(item));
    return list;
}
QVariant PlotAdapter::styledef(const QString& style_item_id) {
    StyleItemValue value = _plot->styledef(style_item_id.toStdString());
    switch(_plot->style_item_type(style_item_id.toStdString())) {
    case StyleItemType::Bool:
        return QVariant(std::get<bool>(value));
    case StyleItemType::String:
        return QVariant(QString::fromStdString(std::get<std::string>(value)));
    case StyleItemType::Integer:
    case StyleItemType::Integer_Range:
        return QVariant(std::get<int>(value));
    case StyleItemType::Double:
    case StyleItemType::Double_Range:
        return QVariant(std::get<double>(value));
    case StyleItemType::Color:
        return QColor(QString::fromStdString(std::get<std::string>(value)));
    case StyleItemType::CustomIcon:
        return QVariant(std::get<int>(value));
    }
    return {};
}
void PlotAdapter::set_styledef(const QString& style_item_id, const QVariant& value) {
    switch(_plot->style_item_type(style_item_id.toStdString())) {
    case StyleItemType::Bool:
        _plot->set_styledef(style_item_id.toStdString(), StyleItemValue(value.toBool()));
        break;
    case StyleItemType::String:
        _plot->set_styledef(style_item_id.toStdString(), StyleItemValue(value.toString().toStdString()));
        break;
    case StyleItemType::Integer:
    case StyleItemType::Integer_Range:
        _plot->set_styledef(style_item_id.toStdString(), StyleItemValue(value.toInt()));
        break;
    case StyleItemType::Double:
    case StyleItemType::Double_Range:
        _plot->set_styledef(style_item_id.toStdString(), StyleItemValue(value.toDouble()));
        break;
    case StyleItemType::Color:
        _plot->set_styledef(style_item_id.toStdString(), StyleItemValue(value.toString().toStdString()));
        break;
    case StyleItemType::CustomIcon:
        _plot->set_styledef(style_item_id.toStdString(), StyleItemValue(value.toInt()));
        break;
    }
    emit styledef_changed(style_item_id, value);
}
void PlotAdapter::reset_styledef() {
    _plot->reset_styledef();
    emit styledef_changed("", "");
}
unsigned int PlotAdapter::features_count() const {
    return _plot->features_count();
}
QString PlotAdapter::feature_label(unsigned int idx) const {
    return QString::fromStdString(_plot->feature_label(idx));
}
unsigned int PlotAdapter::plot_type_count() const {
    return Plot::__static__plots_types_indexes__.size();
}
QString PlotAdapter::plot_type_label(unsigned int idx) const {
    return QString::fromStdString(Plot::__static__plots_types_map__[Plot::__static__plots_types_indexes__[idx]]);
}
void PlotAdapter::set_feature_value(unsigned int idx, const QString& value) {
    _plot->set_feature_value(idx, value.toStdString());
    emit feature_value_changed(idx, value);
}
QString PlotAdapter::feature(unsigned int idx) const {
    return QString::fromStdString(_plot->feature_value(idx));
}