#include "layout_adapter.h"

LayoutAdapter::LayoutAdapter(Layout* layout, QObject* parent): QObject(parent), _layout(layout) {
}

QMap<QString, QList<double>> LayoutAdapter::nodes_bounds() const {
    QMap<QString, QList<double>> map;
    for(auto& [stdid, bounds]: _layout->nodes_bounds()) {
        QString id = QString::fromStdString(stdid);
        map[id] = {};
        for(auto& value: bounds)
            map[id].append(value);
    }
    return map;
}
int LayoutAdapter::get_layout_index() const {
    return _layout->get_index();
}
Graph* LayoutAdapter::graph(const QString& id) {
    return _layout->graph(id.toStdString());
}