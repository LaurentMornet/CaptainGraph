#include "metagraph.h"
#include "../metaplots/metaplot.h"

MetaGraph::MetaGraph() {
    _title = "Graph_" + std::to_string(MetaGraph::__global__id++);
}
std::string & MetaGraph::title() {
    return _title;
}
void MetaGraph::set_title(std::string & title) {
    _title = title;
}
void MetaGraph::add_metaplot(MetaPlot * metaplot) {
    _v_metaplots.push_back(metaplot);
    metaplot->set_ancestor(this);
}
std::string MetaGraph::html_base_2d() {
    QFile file(":/resources/html_base_2d.template");
    if(!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Impossible d'ouvrir le fichier de resource html_base_2d.template");
    }
    QTextStream in(&file);
    QString patron = in.readAll();
    file.close();
    patron.replace("@DATA@", QString("[]"));
    patron.replace("@LAYOUT@", QString("{ hovermode: 'closest', title: {text: '%1'}, margin: { l: 40, r: 10, t: 60, b: 40 }, showlegend: true }").arg(title().c_str()));
    patron.replace("@CONFIG@", QString("{modeBarButtonsToAdd: ['v1hovermode'], modeBarButtonsToRemove: ['select2d','lasso2d'], responsive: true}"));
    return patron.toStdString();
}

MetaPlot * MetaGraph::metaplot(int idx) {
    if(idx >=0 && idx < metaplots_count())
        return _v_metaplots[idx];
    return nullptr;
}

int MetaGraph::metaplots_count() {
    return _v_metaplots.size();
}