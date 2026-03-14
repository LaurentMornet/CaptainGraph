#include "plot.h"
#include "graph.h"
#include <sstream>
#include <iostream>
/* ========================================================================== */
/*                             Constructeurs / copie                          */
/* ========================================================================== */

int Plot::PLOT_NUMBER = -9999;

Plot::Plot(const Plot& src) {
    from(src);
}

Plot::Plot(Graph * parent_graph, std::string type)
    : _graph(parent_graph), 
      _label(""),
      _type(type) {
    if(parent_graph) {
        Plot::PLOT_NUMBER ++;
        std::ostringstream slabel;
        slabel << "Plot " << Plot::PLOT_NUMBER;
        set_label(slabel.str());
    }
}

/**
 * @brief Copie complète depuis un autre Plot.
 */
void Plot::from(const Plot& src) {
    _label = src._label;
    _type = src._type;
    _features_names = src._features_names;
    _features_values = src._features_values;
    _styledef = src._styledef;
}

/**
 * @brief Copie uniquement le styledef (valeurs de style).
 */
void Plot::from_styledef(const Plot& src) {
    _styledef = src._styledef;
}
void Plot::from_styledef(std::map<std::string, StyleItemValue> map) {
    _styledef = map;
}

/**
 * @brief Réinitialise les valeurs de style.
 */
void Plot::raz_styledef() {
    _styledef.clear();
}

/* ========================================================================== */
/*                             Getters / Setters                              */
/* ========================================================================== */

Graph* Plot::graph() const noexcept {
    return _graph;
}

const std::string& Plot::label() const noexcept {
    return _label;
}

void Plot::set_label(const std::string& label) {
    _label = label;
}

std::string Plot::type() const noexcept {
    return _type;
}

void Plot::set_type(std::string type) noexcept {
    _type = type;

}

/* ========================================================================== */
/*                                Style API                                   */
/* ========================================================================== */

std::map<std::string, StyleItemValue> Plot::current_styledef() const {
    return _styledef;
}

void Plot::set_style_item_value(const std::string& id, const StyleItemValue& value) {
    if (id.empty()) {
        throw std::invalid_argument("set_style_item_value(): identifiant vide");
    }
    _styledef[id] = value;
}

std::optional<StyleItemValue> Plot::get_style_item_value(const std::string& id) const noexcept {
    // Vérifie si la clé est explicitement définie
    auto it = _styledef.find(id);
    if (it != _styledef.end()) {
        return it->second;
    }

    // Vérifie dans le modèle du type courant
    auto base_it = base_plots_registry.find(_type);
    if (base_it == base_plots_registry.end()) {
        // registre non chargé
        return std::nullopt;
    }

    const Plot& base_plot = base_it->second;
    auto style_it = base_plot._style.find(id);
    if (style_it != base_plot._style.end()) {
        return style_it->second.default_value;
    }

    // Clé inexistante dans le modèle
    return std::nullopt;
}

const StyleItem& Plot::get_style_item(const std::string& id) const {
    const auto it_type = base_plots_registry.find(_type);
    if (it_type == base_plots_registry.end()) {
        throw std::out_of_range("get_style_item(): type de plot inconnu ou registre non initialisé");
    }

    const auto& base_plot = it_type->second;
    const auto it = base_plot._style.find(id);
    if (it == base_plot._style.end()) {
        throw std::out_of_range("get_style_item(): clé de style '" + id + "' inexistante pour ce type");
    }
    return it->second;
}

std::vector<std::string> Plot::get_style_keys() const {
    auto it_type = base_plots_registry.find(_type);
    if (it_type == base_plots_registry.end()) {
        throw std::runtime_error("get_style_keys(): registre non initialisé");
    }
    return it_type->second._style_keys;
}

std::optional<std::vector<StyleItemArg>> Plot::get_style_item_args(const std::string& id) const noexcept {
    auto it_type = base_plots_registry.find(_type);
    if (it_type == base_plots_registry.end()) {
        return std::nullopt;
    }
    const auto& base_plot = it_type->second;
    auto it = base_plot._style.find(id);
    if (it != base_plot._style.end()) {
        return it->second.args;
    }
    return std::nullopt;
}

/* ========================================================================== */
/*                               Features (X,Y,Z)                             */
/* ========================================================================== */

const std::string& Plot::feature_name(size_t index) const {
    const auto it_type = base_plots_registry.find(_type);
    if (it_type == base_plots_registry.end()) {
        throw std::runtime_error("feature_name(): registre non initialisé");
    }

    const auto& names = it_type->second._features_names;
    if (index >= names.size()) {
        throw std::out_of_range("feature_name(): index hors limites");
    }
    return names[index];
}

std::string& Plot::feature_value(size_t index) {
    const auto it_type = base_plots_registry.find(_type);
    if (it_type == base_plots_registry.end()) {
        throw std::runtime_error("feature_value(): registre non initialisé");
    }

    const auto& names = it_type->second._features_names;
    if (index >= names.size()) {
        throw std::out_of_range("feature_value(): index hors limites");
    }

    // Étend si nécessaire
    if (_features_values.size() < names.size()) {
        _features_values.resize(names.size(), "");
    }

    return _features_values[index];
}

void Plot::set_feature_value(size_t index, const std::string& value) {
    const auto it_type = base_plots_registry.find(_type);
    if (it_type == base_plots_registry.end()) {
        throw std::runtime_error("set_feature_value(): registre non initialisé");
    }

    const auto& names = it_type->second._features_names;
    if (index >= names.size()) {
        throw std::out_of_range("set_feature_value(): index hors limites");
    }

    if (_features_values.size() < names.size()) {
        _features_values.resize(names.size(), "");
    }

    _features_values[index] = value;
}

size_t Plot::features_count() const noexcept {
    auto it_type = base_plots_registry.find(_type);
    if (it_type == base_plots_registry.end()) {
        return 0;
    }
    return it_type->second._features_names.size();
}

/* ========================================================================== */
/*                              Static helpers                                */
/* ========================================================================== */

bool Plot::base_registry_initialized() noexcept {
    return !base_plots_registry.empty();
}

const Plot& Plot::model_for_type(std::string type) {
    if (base_plots_registry.empty()) {
        throw std::runtime_error("model_for_type(): registre non initialisé");
    }

    auto it = base_plots_registry.find(type);
    if (it == base_plots_registry.end()) {
        throw std::out_of_range("model_for_type(): type de plot inconnu");
    }
    return it->second;
}
