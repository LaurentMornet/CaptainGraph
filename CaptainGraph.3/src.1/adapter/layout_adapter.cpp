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
    QString html = in.readAll();
    file.close();

    auto bounds = _layout->nodes_bounds();

    for (auto& [id, b] : bounds) {
        QString qid = QString::fromStdString(id);

        double x = b[0] * 100.0;
        double y = b[1] * 100.0;
        double w = b[2] * 100.0;
        double h = b[3] * 100.0;

        html += QString(
            "<div id='%1' class='plot' style='"
            "left:%2%; top:%3%; width:%4%; height:%5%;'></div>"
        )
        .arg(qid)
        .arg(x)
        .arg(y)
        .arg(w)
        .arg(h);
    }
    // html += "<script>";
    // int i=0;
    // for(auto& [id, b]: bounds) {
    //     html += QString(R"(
    //     Plotly.newPlot('%1', [{
    //         x: [1,2,3],
    //         y: [4,5,6],
    //         type: 'scatter'
    //     }], {
    //         //margin: {l:0, r:0, t:0, b:0}
    //     }, {responsive:true});
    //     )").arg(QString::fromStdString(id));
    //     i++;
    // }
    // html += "</script></body></html>";
    return html;
}