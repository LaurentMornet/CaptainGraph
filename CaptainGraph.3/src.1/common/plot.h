#pragma once

#include <map>
#include <string>
#include <variant>
#include <vector>

enum class PlotType {
    Scatter,
    Line,
    Stem,
    Bar,
    Polar,
    Scatter3D,
    Pie,
    HLine,
    VLine,
    PColorMesh
};

enum class StyleItemType {
    Bool,
    Integer,
    Double,
    String,
    Integer_Range,
    Double_Range,
    Color,
    CustomIcon
};

using StyleItemValue = std::variant<bool, int, double, std::string>;

class StyleItem {
private:
    std::string                 _id;
    StyleItemType               _type;
    std::string                 _label;
    StyleItemValue              _initial;
    std::vector<StyleItemValue> _args;
public:
    explicit StyleItem(
        const std::string& id,
        const StyleItemType& type, 
        const std::string& label, 
        const StyleItemValue& initial, 
        const std::vector<StyleItemValue>& args={});
    virtual ~StyleItem() = default;
    // accesseurs
    std::string id() const;
    StyleItemType type() const;
    std::string label() const;
    StyleItemValue initial() const;
    unsigned int args_count() const;
    StyleItemValue arg(const unsigned int& idx) const;
    // modificateurs
    // on verra si besoin...
};

using StyleDatabase = std::map<PlotType, std::vector<StyleItem>>;

class Plot {
private:
    static int __static__id_count__;
    static std::map<PlotType, std::vector<std::string>> __static__features__;
    PlotType _type;
    std::map<std::string, StyleItemValue> _styledef;
    std::vector<std::string> _features_ids;
    int _id;
    std::string _label;
    std::vector<std::string> _features;
public:
    static StyleDatabase __static__style_database__;
    static std::map<PlotType, std::string> __static__plots_types_map__;
    static std::map<PlotType, std::string> __static__types_icons__;
    static std::vector<PlotType> __static__plots_types_indexes__;
    static std::map<std::string, std::vector<std::string>> __static__custom_icons__;
    explicit Plot();
    virtual ~Plot()=default;
    PlotType type() const;
    void set_type(const PlotType& type);
    int id() const;
    std::string label() const;
    void set_label(const std::string& label);
    StyleItemValue styledef(const std::string& style_item_id) const;
    StyleItemType style_item_type(const std::string& style_item_id) const;
    void set_styledef(const std::string& style_item_id, StyleItemValue style_item_value);
    void reset_styledef();
    std::vector<std::string> style_keys() const;

    unsigned int features_count() const;
    std::string feature_label(const unsigned int& idx) const;
    std::string feature_value(const unsigned int& idx) const;
    void set_feature_value(const unsigned int& idx, const std::string& value);
};