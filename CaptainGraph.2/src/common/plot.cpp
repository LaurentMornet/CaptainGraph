#include "plot.h"
#include <sstream>
/**
 * Définition Style
 */
StyleItem::StyleItem(
    const std::string& id,
    const StyleItemType& type, 
    const std::string& label,
    const StyleItemValue& initial, 
    const std::vector<StyleItemValue>& args): 
    _id(id), _type(type), _label(label), _initial(initial), _args(args) {}
std::string StyleItem::id() const { return _id; }
StyleItemType StyleItem::type() const { return _type; }
std::string StyleItem::label() const { return _label; }
StyleItemValue StyleItem::initial() const { return _initial; }
unsigned int StyleItem::args_count() const { return _args.size(); }
StyleItemValue StyleItem::arg(const unsigned int& idx) const { return _args[idx]; }

/** 
 * Définition Plot
 */
int Plot::__static__id_count__ = 0;

Plot::Plot() {
    Plot::__static__id_count__ ++;
    std::ostringstream sid;
    sid << "plot_" << Plot::__static__id_count__;
    set_label(sid.str());
    set_type(PlotType::Scatter);
}
std::string Plot::label() const { return _label; }
void Plot::set_label(const std::string& label) { _label = label; }
PlotType Plot::type() const { return _type; }
void Plot::set_type(const PlotType& type) {
    if(type == _type) return;
    _type = type;
    _features_ids = Plot::__static__features__[type];
    while(_features_ids.size() > _features.size()) _features.emplace_back("");
}
StyleItemValue Plot::styledef(const std::string& style_item_id) const {
    if(_styledef.count(style_item_id) > 0)
        return _styledef.at(style_item_id);
    for(auto& item: Plot::__static__style_database__.at(type()))
        if(item.id() == style_item_id)
            return item.initial();
    return StyleItemValue(-1);
}
void Plot::set_styledef(const std::string& style_item_id, StyleItemValue style_item_value) {
    _styledef[style_item_id] = style_item_value;
}
void Plot::reset_styledef() {
    _styledef.clear();
}
StyleItemType Plot::style_item_type(const std::string& style_item_id) const {
    for(auto& item: Plot::__static__style_database__.at(type()))
        if(item.id() == style_item_id)
            return item.type();
    return StyleItemType(false);
}
std::vector<std::string> Plot::style_keys() const {
    std::vector<std::string> list;
    for(auto& item: Plot::__static__style_database__.at(type()))
        list.emplace_back(item.id());
    return list;
}
unsigned int Plot::features_count() const {
    return _features_ids.size();
}
std::string Plot::feature_label(const unsigned int& idx) const { 
    return _features_ids[idx];
}
std::string Plot::feature_value(const unsigned int& idx) const {
    return _features[idx];
}
void Plot::set_feature_value(const unsigned int& idx, const std::string& value) {
    _features[idx] = value;
}