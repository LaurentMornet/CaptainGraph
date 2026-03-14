#include "graph_frame.h"

Bridge::Bridge(QWidget * parent)
: QObject(parent) {
}
void Bridge::mouse_pressed() {
    GraphFrame * graph_frame = qobject_cast<GraphFrame*>(this->parent());
    if(GraphConfigFrame::getInstance()->metagraph() != graph_frame->metagraph()) {
        emit GraphConfigFrame::getInstance()->selected(graph_frame->metagraph());
        if(graph_frame->metagraph()->metaplots_count() > 0)
            emit PlotConfigFrame::getInstance()->selected(graph_frame->metagraph()->metaplot(0));
    }
}

GraphFrame::GraphFrame(QWidget * parent)
    : QFrame(parent) {
    _bridge = new Bridge(this);
    setFrameShape(QFrame::Shape::StyledPanel);
    // moc
    _metagraph.add_metaplot(new ScatterMetaPlot());

    auto * layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
    _view = new QWebEngineView(this);
    auto * channel = new QWebChannel(_view);
    _view->page()->setWebChannel(channel);
    channel->registerObject("bridge", _bridge);
    _view->setHtml(_metagraph.html_base_2d().c_str());
    QVector<double> x = {1, 2, 3, 4, 5};
    QVector<double> yBar = {2, 5, 7, 3, 6};
    QVector<double> yLine = {1, 4, 6, 2, 7};
    QObject::connect(_view, &QWebEngineView::loadFinished, [=](bool) {
        // QString jsCode = QString(_metagraph.html_base_2d().c_str());
        // QString jsCode = QString(
        //     "Plotly.react('plot', ["
        //     "{ x: %1, y: %2, type: 'bar', name: 'Histogramme' },"
        //     "{ x: %1, y: %3, type: 'scatter', mode: 'lines+markers', line: {color: 'red'}, name: 'Courbe' }"
        //     "], { hovermode: 'closest', title: {text: '%4'}, margin: { l: 40, r: 10, t: 60, b: 40 }, showlegend: true },"
        //     "{modeBarButtonsToAdd: ['v1hovermode'], modeBarButtonsToRemove: ['select2d','lasso2d'], responsive: true});"
        // ).arg(_to_js_array(x), _to_js_array(yBar), _to_js_array(yLine), _metagraph.title().c_str());
        // _view->page()->runJavaScript(jsCode);
    });
    layout->addWidget(_view);



    update_graph();
}

QString GraphFrame::_to_js_array(const QVector<double>& data) {
    QString js = "[";
    for (double v: data)
        js += QString::number(v) + ",";
    if (!data.isEmpty())
        js.chop(1);
    js += "]";
    return js;
}  

void GraphFrame::update_graph() {
}

MetaGraph * GraphFrame::metagraph() {
    return &_metagraph;
}