#include "plot.h"
#include <tinyxml2.h>
#include <iostream>
#include <sstream>

using namespace tinyxml2;

std::vector<std::string> Plot::PlotTypeNames = {};
std::map<std::string, std::vector<std::string>> Plot::CustomIconTypes = {};
std::map<std::string, Plot> Plot::base_plots_registry = {};

static StyleItemType parse_style_item_type(const std::string& type_str) {
    if (type_str == "bool") return StyleItemType::BOOL;
    if (type_str == "string") return StyleItemType::STRING;
    if (type_str == "int-range") return StyleItemType::INT_RANGE;
    if (type_str == "double-range") return StyleItemType::DOUBLE_RANGE;
    if (type_str == "int") return StyleItemType::INTEGER;
    if (type_str == "double") return StyleItemType::DOUBLE;
    if (type_str == "color") return StyleItemType::COLOR;
    if (type_str == "custom-icon") return StyleItemType::CUSTOM_ICON;

    // Cas ambigu : référence à un type custom-icon via son nom
    if (!type_str.empty())
        return StyleItemType::CUSTOM_ICON;

    return StyleItemType::STRING;
}

static StyleItemMode parse_style_item_mode(const std::string& mode_str) {
    return (mode_str == "advanced") ? StyleItemMode::ADVANCED : StyleItemMode::BASIC;
}

void Plot::load_base_plots_registry() {
    XMLDocument doc;
    if (doc.LoadFile("resources/registry.xml") != XML_SUCCESS)
        throw std::runtime_error("Impossible de charger le fichier plot_registry.xml");

    XMLElement* root = doc.FirstChildElement("registry");
    if (!root)
        throw std::runtime_error("XML invalide : pas de balise <registry>");

    /* ----------- CUSTOM ICON TYPES ----------- */
    Plot::CustomIconTypes.clear();
    XMLElement* customIconTypes = root->FirstChildElement("custom-icon-types");
    if (customIconTypes) {
        for (XMLElement* customType = customIconTypes->FirstChildElement("custom-icon-type");
             customType;
             customType = customType->NextSiblingElement("custom-icon-type"))
        {
            const char* typeId = customType->Attribute("id");
            if (!typeId) continue;

            std::vector<std::string> icons;
            for (XMLElement* item = customType->FirstChildElement("custom-style-type-item");
                 item;
                 item = item->NextSiblingElement("custom-style-type-item"))
            {
                const char* iconPath = item->Attribute("icon");
                if (iconPath)
                    icons.emplace_back(std::string(":/resources/") + iconPath);
            }

            Plot::CustomIconTypes[typeId] = icons;
        }
    }

    /* ----------- PLOT TYPES ----------- */
    Plot::base_plots_registry.clear();
    Plot::PlotTypeNames.clear();

    XMLElement* plotTypes = root->FirstChildElement("plot-types");
    if (!plotTypes)
        throw std::runtime_error("XML invalide : pas de balise <plot-types>");

    for (XMLElement* plotType = plotTypes->FirstChildElement("plot-type");
         plotType;
         plotType = plotType->NextSiblingElement("plot-type"))
    {
        const char* id = plotType->Attribute("id");
        if (!id) continue;

        Plot p(nullptr, id);
        Plot::PlotTypeNames.push_back(id);

        // Lecture des styles
        XMLElement* style = plotType->FirstChildElement("style");
        if (style) {
            for (XMLElement* styleItem = style->FirstChildElement("style-item");
                 styleItem;
                 styleItem = styleItem->NextSiblingElement("style-item"))
            {
                const char* sid   = styleItem->Attribute("id");
                const char* type  = styleItem->Attribute("type");
                const char* mode  = styleItem->Attribute("mode");
                const char* label = styleItem->Attribute("label");
                const char* init  = styleItem->Attribute("initial");

                if (!sid || !type || !label || !init)
                    continue;

                StyleItem item;
                item.type  = parse_style_item_type(type);
                item.mode  = parse_style_item_mode(mode ? mode : "basic");
                item.label = label;

                std::string initStr(init);
                if (item.type == StyleItemType::INTEGER || item.type == StyleItemType::INT_RANGE || item.type == StyleItemType::CUSTOM_ICON)
                    item.default_value = std::stoi(initStr);
                else if (item.type == StyleItemType::DOUBLE || item.type == StyleItemType::DOUBLE_RANGE)
                    item.default_value = std::stod(initStr);
                else if (item.type == StyleItemType::BOOL)
                    item.default_value = (initStr == "true" || initStr == "1");
                else
                    item.default_value = initStr;

                // Lecture des <style-item-arg>
                for (XMLElement* arg = styleItem->FirstChildElement("style-item-arg");
                     arg; arg = arg->NextSiblingElement("style-item-arg"))
                {
                    const char* val = arg->Attribute("value");
                    if (!val) continue;
                    std::string v(val);

                    // Essai de conversion numérique, sinon string
                    try {
                        if (v.find('.') != std::string::npos)
                            item.args.push_back(StyleItemArg(std::stod(v)));
                        else
                            item.args.push_back(StyleItemArg(std::stoi(v)));
                    } catch (...) {
                        item.args.push_back(StyleItemArg(v));
                    }
                }

                p._style_keys.push_back(sid);
                p._style[sid] = item;
            }
        }

        XMLElement* features = plotType->FirstChildElement("features");
        if (features) {
            for (XMLElement* feature_item = features->FirstChildElement("feature");
                 feature_item;
                 feature_item = feature_item->NextSiblingElement("feature"))
            {
                const char* label   = feature_item->Attribute("label");
                p._features_names.push_back(label);
                p._features_values.push_back("");
            }
        }
        Plot::base_plots_registry[id] = p;
    }

    std::cout << "✅ Chargement du registre XML terminé : "
              << Plot::base_plots_registry.size() << " types de plots." << std::endl;
}
