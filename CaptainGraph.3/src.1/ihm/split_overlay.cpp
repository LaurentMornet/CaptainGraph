#include "split_overlay.h"
#include "layout_panel.h"
#include "layout_frame.h"

#include <QSplitter>
#include <QTimer>
#include <QVBoxLayout>
#include <QLabel>
#include <cmath>

SplitOverlay::SplitOverlay(QWidget* parent, LayoutFrame* layout_frame)
    : QWidget(parent), _layout_frame(layout_frame) {
    setContentsMargins(0,0,0,0);
}
void SplitOverlay::set_layout_adapter(LayoutAdapter* layout_adapter) {
    _layout_adapter = layout_adapter;
    rebuild();
}
void SplitOverlay::clear_root_widget() {
    if (!_rootWidget) return;

    _rootWidget->hide();
    _rootWidget->setParent(nullptr);
    _rootWidget->deleteLater();
    _rootWidget = nullptr;
}
void SplitOverlay::rebuild() {
    if(_rebuild_pending) return;
    _rebuild_pending = true;
    clear_root_widget();
    if (!_layout_adapter) return;
    Layout* rootLayout = _layout_adapter->layout_ptr();
    if (!rootLayout) return;

    _rootWidget = build_widget_from_layout(rootLayout);
    if (!_rootWidget) return;
    delete this->layout();
    auto* lay = new QVBoxLayout(this);
    lay->setContentsMargins(0,0,0,0);
    lay->addWidget(_rootWidget);
    setLayout(lay);

    _layout_frame->rebuild_plotly();
    _rebuild_pending = false;
}
void SplitOverlay::resizeEvent(QResizeEvent* e) {
    QWidget::resizeEvent(e);
    if (_rootWidget) _rootWidget->resize(size());
}
QWidget* SplitOverlay::build_widget_from_layout(Layout* layout) {
    if (!layout) return nullptr;

    if (layout->type() == LayoutType::Node) {
        // create panel identified by layout->id()
        QString qid = QString::fromStdString(layout->id());
        auto* panel = new LayoutPanel(_layout_adapter, qid, this);
        // connect signals from panel to actions that modify the model via the adapter
        connect(panel, &LayoutPanel::request_split, this, [this](const QString& layout_id, LayoutType direction) {
            if (!_layout_adapter) return;
            _layout_adapter->split(layout_id, direction);
            for(auto item: _layout_adapter->siblings(layout_id)) {
                qDebug() << _layout_adapter->weight(item);
            }
            qDebug() << "---";
            // emit split_occured();
            rebuild();
        });
        connect(panel, &LayoutPanel::request_merge, this, [this](const QString& layout_id, MergeDirection direction) {
            if(!_layout_adapter) return;
            _layout_adapter->merge(layout_id, direction);
            rebuild();
        });
        return panel;
    } else {
        Qt::Orientation ori = (layout->type() == LayoutType::Horizontal) ? Qt::Horizontal : Qt::Vertical;
        QSplitter* splitter = new QSplitter(ori);
        _splitter_map.insert(splitter, {});
        _splitter_mapw.insert(splitter, {});
        splitter->setHandleWidth(2);
        splitter->setStyleSheet("QSplitter:handle { background-color: gray; }");

        connect(splitter, &QSplitter::splitterMoved, this, &SplitOverlay::_cb_splitter_moved);

        const auto& children = layout->sublayouts();
        const auto& weights = layout->absolute_weights();

        for (size_t i = 0; i < children.size(); ++i) {
            QWidget* childWidget = build_widget_from_layout(children[i].get());
            if (!childWidget) childWidget = new QLabel("?");
            _splitter_map[splitter].append(QString::fromStdString(children[i]->id()));
            _splitter_mapw[splitter].append(childWidget);
            splitter->addWidget(childWidget);
        }
        // apply stretch factors approximately from weights
        for(int i = 0; i < splitter->count(); i++) {
            splitter->setStretchFactor(i, 100.0 * weights[i]);
        }
        return splitter;
    }

}
void SplitOverlay::_cb_splitter_moved(int pos, int index) { 
    Q_UNUSED(pos) 
    QSplitter* splitter = qobject_cast<QSplitter*>(sender());
    double pw = (double)splitter->width(); 
    double ph = (double)splitter->height(); 
    double cw = (double)_splitter_mapw[splitter][index - 1]->width(); 
    double ch = (double)_splitter_mapw[splitter][index - 1]->height(); 
    double weight = pw == cw ? ch / ph : cw / pw;
    _layout_adapter->set_weight(_splitter_map[splitter][index-1], weight);
    _layout_frame->rebuild_plotly(); 
}