#include "mini_layout_frame.h"
#include "main_window.h"
#include "ihm_tools.h"
/**
 * Layout Render Widget implementation
 */
MiniLayoutRenderWidget::MiniLayoutRenderWidget(MiniLayoutFrame* parent): QWidget(parent), _mini_layout_frame(parent) {
    _current_id = "";
    setMinimumHeight(80);
}
QSize MiniLayoutRenderWidget::sizeHint() const { 
    return {100, 60}; 
}
void MiniLayoutRenderWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)
    if(!_mini_layout_frame->layout_adapter()) return;
    QPainter* painter = new QPainter(this);
    QMap<QString, QList<double>> map = _mini_layout_frame->layout_adapter()->nodes_bounds();
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
        if(IhmTools::is_dark_mode()) {
            if(it.key() == _current_id) {
                painter->fillRect(x, y, w, h, Qt::white);
                painter->setPen(Qt::black);
                painter->drawRect(x, y, w, h);
                painter->drawText(x, y, w, h, Qt::AlignCenter, it.key());
            }
            else {
                painter->setPen(Qt::white);
                painter->drawRect(x, y, w, h);
                painter->drawText(x, y, w, h, Qt::AlignCenter, it.key());
            }
        }
        else {
            if(it.key() == _current_id) {
                painter->fillRect(x, y, w - 1, h - 1, Qt::white);
                painter->setPen(Qt::black);
                painter->drawRect(x, y, w, h);
                painter->drawText(x, y, w, h, Qt::AlignCenter, it.key());
            }
            else {
                painter->setPen(Qt::black);
                painter->drawRect(x, y, w, h);
                painter->drawText(x, y, w, h, Qt::AlignCenter, it.key());
            }
        }
    }
    painter->end();
}
void MiniLayoutRenderWidget::mousePressEvent(QMouseEvent* event) {
    double x = (double)event->pos().x() / (double)size().width();
    double y = (double)event->pos().y() / (double)size().height();
    QMap<QString, QList<double>> map = _mini_layout_frame->layout_adapter()->nodes_bounds();
    for (auto it = map.cbegin(), end = map.cend(); it != end; ++it) {
        if(x>=it.value()[0] && x<=it.value()[0]+it.value()[2] && y>=it.value()[1] && y<=it.value()[1]+it.value()[3]) {
            if(it.key() != _current_id) {
                _current_id = it.key();
                repaint();
                emit layout_selected(it.key());
            }
            break;
        }
    }
}
/**
 * Layout Frame implementation
 */
QSize MiniLayoutFrame::sizeHint() const {
    return {200, 150}; 
}
MiniLayoutFrame::MiniLayoutFrame(MainWindow* parent): 
    QFrame(parent), 
    _main_window(parent) {
    auto * main_layout = new QVBoxLayout(this);
    setLayout(main_layout);
    _main_frame = new QFrame(this);
    main_layout->addWidget(_main_frame);
    auto * layout = new QVBoxLayout(_main_frame);
    layout->setContentsMargins(0, 0, 0, 0);
    _render_widget = new MiniLayoutRenderWidget(this);
    _main_frame->setLayout(layout);
    layout->addWidget(_render_widget);
    connect(_render_widget, &MiniLayoutRenderWidget::layout_selected, this, &MiniLayoutFrame::_cb_layout_selected);
}
LayoutAdapter* MiniLayoutFrame::layout_adapter() const { 
    return _layout_adapter; 
}
void MiniLayoutFrame::set_layout_adapter(LayoutAdapter * layout_adapter) {
    if(!layout_adapter) {
        _main_frame->setVisible(false);
        return;
    }
    _layout_adapter = layout_adapter;
    disconnect(_layout_adapter, nullptr, nullptr, nullptr);
    _main_frame->setVisible(true);
    _main_frame->repaint();
    connect(_layout_adapter, &LayoutAdapter::weights_changed, this, &MiniLayoutFrame::_cb_weights_changed);
}
void MiniLayoutFrame::_cb_layout_selected(const QString& layout_id) {
    _main_window->graph_frame()->set_graph_adapter(new GraphAdapter(_layout_adapter->graph(layout_id), _main_window->graph_frame()));
}
void MiniLayoutFrame::_cb_weights_changed() {
    _render_widget->setVisible(false);
    _render_widget->setVisible(true);
}