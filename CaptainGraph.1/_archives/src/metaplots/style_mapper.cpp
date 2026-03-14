#include "style_mapper.h"

QVariantMap StyleMapper::to_map(const BaseStyle* base) {
    QVariantMap map;
    if (auto s = dynamic_cast<const ScatterStyle*>(base)) {
        map["line_color"] = QString(s->line_color.c_str());
        map["line_style"] = QString(s->line_style.c_str());
        map["marker_size"] = s->marker_size;
        map["show_legend"] = s->show_legend;
    }
    return map;
}
std::unique_ptr<BaseStyle> StyleMapper::from_map(const QString& type, const QVariantMap& map) {
    if (type == "Scatter") {
        auto s = std::make_unique<ScatterStyle>();
        s->line_color = map.value("line_color", "#0000ff").toString().toStdString();
        s->line_style = map.value("line_style", "solid").toString().toStdString();
        s->marker_size = map.value("marker_size", 5).toInt();
        s->show_legend = map.value("show_legend", true).toBool();
        return s;
    }
    return nullptr;
}
