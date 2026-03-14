#include "layout_adapter.h"
#include <QDebug>
#include <QFile>
#include <functional>

LayoutAdapter::LayoutAdapter(Layout* layout, QObject* parent)
    : QObject(parent), _layout(layout) {
}
QMap<QString, QList<double>> LayoutAdapter::nodes_bounds() const {
    QMap<QString, QList<double>> map;
    if (!_layout) return map;
    for (auto& p : _layout->nodes_bounds()) {
        const std::string& stdid = p.first;
        const double* bounds = p.second;
        QString id = QString::fromStdString(stdid);
        QList<double> list;
        list << bounds[0] << bounds[1] << bounds[2] << bounds[3];
        map[id] = list;
    }
    return map;
}
int LayoutAdapter::get_layout_index() const {
    if (!_layout) return -1;
    return _layout->get_index();
}
Graph* LayoutAdapter::graph(const QString& id) {
    if (!_layout) return nullptr;
    return _layout->graph(id.toStdString());
}
Layout* LayoutAdapter::layout_ptr() const { 
    return _layout; 
}
Layout* LayoutAdapter::find(const QString& id) const {
    if (!_layout) return nullptr;

    const std::string target = id.toStdString();

    // Nom unique pour éviter collision de nom avec d'autres symboles
    std::function<Layout*(Layout*)> recurse = [&](Layout* node) -> Layout* {
        if (!node) return nullptr;
        if (node->id() == target) return node;
        for (const auto& child_ptr : node->sublayouts()) {
            if (Layout* found = recurse(child_ptr.get())) return found;
        }
        return nullptr;
    };

    return recurse(_layout);
}
void LayoutAdapter::set_weight(const QString& id, double new_weight) {
    if(!_layout) return;
    find(id)->set_weight(new_weight);
    emit weights_changed();
}
double LayoutAdapter::weight(const QString& id) const {
    if(!_layout) return 0.0;
    return find(id)->weight();
}
void LayoutAdapter::split(const QString& id, LayoutType type) {
    find(id)->split(type);
    emit split_occured();
}
QList<QString> LayoutAdapter::siblings(const QString& id) {
    QList<QString> list;
    Layout * this_layout = find(id);
    if(this_layout) {    
        for(auto& item: find(id)->ancestor()->sublayouts())
            list.append(QString::fromStdString(item->id()));
    }
    return list;
}
bool LayoutAdapter::can_merge(const QString& id, MergeDirection direction) {
    Layout * this_layout = find(id);
    if(!this_layout) return false;
    return this_layout->can_merge(direction);
}
void LayoutAdapter::merge(const QString& id, MergeDirection direction) {
    Layout * this_layout = find(id);
    if(!this_layout) return;
    QList<QString> siblings_list = siblings(id);
    this_layout->merge(direction);
    emit merge_occured();
}
QString LayoutAdapter::to_plotly_html() {
    QFile file(":/resources/html.template");
    if(!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Impossible d'ouvrir le fichier de resource html_base_2d.template");
    }
    QTextStream in(&file);
    QString patron = in.readAll();
    file.close();

    constexpr double M = 0.01;
    QStringList buffer_layout;
    int i=1;
    for(auto& [key, item]: _layout->nodes_bounds()) {
        if(find(QString::fromStdString(key))->type() == LayoutType::Node) {
            buffer_layout << QString("xaxis%1: { domain: [%2, %3], anchor: 'y%1', showticklabels: true}").
                arg(i).
                arg(item[0] + M).
                arg(item[0] + item[2] - M);
            buffer_layout << QString("yaxis%1: { domain: [%2, %3], anchor: 'x%1', showticklabels: true}").
                arg(i).
                arg(1-(item[1] + item[3] + M)).
                arg(1-(item[1] - M));
            i++;
        }
    }

    QStringList buffer_data;
    QStringList data_list;
    for(int j=0; j<i-1; j++) {
        buffer_data << QString("var trace%1 = { x: [1, 2, 3], y: [4, 5, 6], xaxis: 'x%2', yaxis: 'y%3', type:'scatter'};")
            .arg(j + 1).arg(j + 1).arg(j + 1);
        data_list << QString("trace%1").arg(j + 1);
    }

    patron.replace("@LAYOUT_DEF@", buffer_layout.join(","));
    patron.replace("@DATA_DEF@", buffer_data.join("\n"));
    patron.replace("@DATA@", data_list.join(","));
    // patron.replace("@LAYOUT@", QString("{ hovermode: 'closest', title: {text: '%1'}, margin: { l: 40, r: 10, t: 60, b: 40 }, showlegend: true }").arg(""));
    patron.replace("@LAYOUT@", QString("{ hovermode: 'closest', title: {text: '%1'}, margin: { l: 0, r: 0, t: 0, b: 0 }, showlegend: true }").arg(""));
    patron.replace("@CONFIG@", QString("{modeBarButtonsToAdd: ['v1hovermode'], modeBarButtonsToRemove: ['select2d','lasso2d'], responsive: true}"));

    return patron;
}