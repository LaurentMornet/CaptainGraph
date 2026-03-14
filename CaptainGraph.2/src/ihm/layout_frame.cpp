#include "layout_frame.h"
#include "main_window.h"
/**
 * Layout Render Widget implementation
 */
LayoutRenderWidget::LayoutRenderWidget(QWidget* parent): QWidget(parent), _layout_adapter(nullptr) {
    _current_id = "";
    setMinimumHeight(80);
}
void LayoutRenderWidget::set_layout_adapter(LayoutAdapter* layout_adapter) {
    _layout_adapter = layout_adapter;
}
QSize LayoutRenderWidget::sizeHint() const { return QSize(100, 60); }
void LayoutRenderWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)
    if(!_layout_adapter) return;
    QPainter* painter = new QPainter(this);
    QMap<QString, QList<double>> map = _layout_adapter->nodes_bounds();
    int offset_x = 0;
    int offset_y = 0;
    int x, y, w, h;
    int width = size().width();
    int height = size().height();
    painter->drawRect(0, 0, width-1, height-1);
    for (auto it = map.cbegin(), end = map.cend(); it != end; ++it) {
        x = offset_x + it.value()[0] * width;
        y = offset_y + it.value()[1] * height;
        w = it.value()[2] * width;
        h = it.value()[3] * height;
        if(it.key() == _current_id) {
            painter->fillRect(x, y, w, h, Qt::white);
            painter->setPen(Qt::black);
            painter->drawText(x, y, w, h, Qt::AlignCenter, it.key());
            painter->setPen(Qt::white);
        }
        else {
            painter->drawRect(x, y, w, h);
            painter->drawText(x, y, w, h, Qt::AlignCenter, it.key());
        }
    }
    painter->end();
}
void LayoutRenderWidget::mousePressEvent(QMouseEvent* event) {
    double x = (double)event->pos().x() / (double)size().width();
    double y = (double)event->pos().y() / (double)size().height();
    QMap<QString, QList<double>> map = _layout_adapter->nodes_bounds();
    for (auto it = map.cbegin(), end = map.cend(); it != end; ++it) {
        if(x>=it.value()[0] && x<=it.value()[0]+it.value()[2] && y>=it.value()[1] && y<=it.value()[1]+it.value()[3]) {
            _current_id = it.key();
            repaint();
            emit layout_selected(it.key());
            // int layout_idx = _layout_adapter->get_layout_index();
            // if(layout_idx >= 0) {
            //     Graph* graph = _layout_adapter->graph(it.key());
            //     _main_window->graph_frame()->set_graph_adapter(new GraphAdapter(graph, _main_window->graph_frame()));
            // }
            break;
        }
    }
}
/**
 * Layout Frame implementation
 */
QSize LayoutFrame::sizeHint() const { return QSize(200, 150); }
LayoutFrame::LayoutFrame(MainWindow* parent): QFrame(parent), _main_window(parent) {
    auto * main_layout = new QVBoxLayout(this);
    setLayout(main_layout);
    // main_layout->setContentsMargins(0, 0, 0, 0);
    _main_frame = new QFrame(this);
    main_layout->addWidget(_main_frame);

    auto * layout = new QVBoxLayout(_main_frame);
    layout->setContentsMargins(0, 0, 0, 0);
    _render_widget = new LayoutRenderWidget(this);
    _main_frame->setLayout(layout);
    layout->addWidget(_render_widget);
    connect(_render_widget, &LayoutRenderWidget::layout_selected, this, &LayoutFrame::_cb_layout_selected);
}
void LayoutFrame::set_layout_adapter(LayoutAdapter * layout_adapter) {
    if(!layout_adapter) {
        _main_frame->setVisible(false);
        return;
    }
    _layout_adapter = layout_adapter;
    _render_widget->set_layout_adapter(_layout_adapter);
    _main_frame->setVisible(true);
    _main_frame->repaint();
}
void LayoutFrame::_cb_layout_selected(const QString& layout_id) {
    _main_window->graph_frame()->set_graph_adapter(new GraphAdapter(_layout_adapter->graph(layout_id), _main_window->graph_frame()));
}