#include "configuration_frame.h"
#include "main_window.h"

#include <cmath>
#include <QGraphicsRectItem>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QToolButton>
#include <QPainter>
// -----------------
// GridGraphicsScene
// -----------------
GridGraphicsScene::GridGraphicsScene(ConfigurationFrame* parent)
    : QGraphicsScene(parent),
    _configuration_frame(parent),
    _visible(true) {
}
void GridGraphicsScene::update_scene() {
    update();
}
void GridGraphicsScene::set_grid_visible(const bool& visible) {
    _visible = visible;
    update_scene();
}
void GridGraphicsScene::drawBackground(QPainter* painter, const QRectF& rect) {
    if (!_visible)
        return;

    double grid_step = _configuration_frame->grid_step();

    painter->setRenderHint(QPainter::Antialiasing, false);
    QColor dot_color(255, 255, 255, 60);
    painter->setPen(Qt::NoPen);
    painter->setBrush(dot_color);
    double left   = std::floor(rect.left()   / grid_step) * grid_step;
    double top    = std::floor(rect.top()    / grid_step) * grid_step;
    double right  = rect.right();
    double bottom = rect.bottom();

    for (double x = left; x < right; x += grid_step) {
        for (double y = top; y < bottom; y += grid_step) {
            painter->drawEllipse(QPointF(x, y), 1.3, 1.3);
        }
    }
}
// ------------------
// ConfigurationFrame
// ------------------
ConfigurationFrame::ConfigurationFrame(MainWindow* main_window)
    :QWidget(main_window),
    _configuration_adapter(nullptr),
    _current_page(-1),
    _grid_step(10.0) {

    auto* layout = new QVBoxLayout(this);
    this->setLayout(layout);
    _toolbar = new QToolBar(this);
    layout->addWidget(_toolbar);

    auto* content = new QFrame(this);
    layout->addWidget(content);
    auto* stack_layout = new QStackedLayout(content);
    stack_layout->setStackingMode(QStackedLayout::StackAll);
    _webview = new QWebEngineView(content);
    _webview->setAttribute(Qt::WA_NativeWindow, false);
    _webview->page()->setBackgroundColor(Qt::transparent);
    _graphics_view = new QGraphicsView(content);
    stack_layout->addWidget(_webview);
    stack_layout->addWidget(_graphics_view);
    _webview->setVisible(false);
    _graphics_view->setVisible(false);

    _graphics_view->setStyleSheet("background: transparent;");
    _graphics_view->setAttribute(Qt::WA_TranslucentBackground);
    _graphics_view->setFrameShape(QFrame::NoFrame);
    _graphics_view->viewport()->setAttribute(Qt::WA_TranslucentBackground);
    _graphics_view->setBackgroundBrush(Qt::NoBrush);

    _graphics_view->setAutoFillBackground(false);
    _graphics_view->viewport()->setAutoFillBackground(false);
    _graphics_view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    _graphics_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _graphics_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _graphics_view->raise();

    _graphics_scene = new GridGraphicsScene(this);
    _graphics_scene->set_grid_visible(false);
    _graphics_view->setScene(_graphics_scene);

    _graphics_scene->setSceneRect(0, 0, 5000, 5000);
    _graphics_view->setScene(_graphics_scene);
}
void ConfigurationFrame::set_grid_step(const double& grid_step) {
    _grid_step = grid_step;
}
double ConfigurationFrame::grid_step() const {
    return _grid_step;
}
void ConfigurationFrame::set_configuration_adapter(ConfigurationAdapter* conf_adapter) {
    if(_configuration_adapter != nullptr) {
        for(auto& action: _actions)
            _toolbar->removeAction(action);
        _actions.clear();
    }
    _configuration_adapter = conf_adapter;
    if(_configuration_adapter) {
        _webview->setVisible(true);
        _webview->setHtml("<html><body><div style=\"position: absolute; left: 40px; top: 40px; width: 100px; height: 100px; border: solid 1px white;\"></div></body></html>");
        _graphics_view->setVisible(true);
        _graphics_scene->clear();
        for(short i = 0; i < _configuration_adapter->pages_count(); i++)
            _actions.append(_toolbar->addAction(
                _configuration_adapter->page_adapter(i)->label(),
                this,
                [this, i]() { set_current_page(i); }
            ));
        set_current_page(0);
    }
}
void ConfigurationFrame::set_current_page(const short& ipage) {
    if(!_configuration_adapter)
        return;
    _current_page = ipage;
    _graphics_scene->clear();
    _rectangles.clear();
    QSizeF viewSize = _graphics_view->viewport()->size();
    double width  = viewSize.width();
    double height = viewSize.height();
    _graphics_scene->setSceneRect(0, 0, width, height);
    // Récupérer le pas de grille
    for(short i = 0; i < _configuration_adapter->page_adapter(ipage)->rectangles_count(); i++) {
        auto rect = _configuration_adapter->page_adapter(ipage)->rectangle(i);
        // Dénormalisation
        QRectF sceneRect(
            width  * rect->left,
            height * rect->top,
            width  * rect->width,
            height * rect->height
        );
        // Appliquer le snap après dénormalisation
        sceneRect = ResizableSnapRectItem::snapped_rect(sceneRect, grid_step());
        _rectangles.append(sceneRect);
        auto* gi = new ResizableSnapRectItem(i, sceneRect, nullptr, this);
        connect(gi, &ResizableSnapRectItem::rectangle_changed,
                this, &ConfigurationFrame::onRectangleChanged);
        _graphics_scene->addItem(gi);
    }
}
void ConfigurationFrame::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    if(!_graphics_view || !_graphics_scene)
        return;
    // QSizeF size = _graphics_view->viewport()->size();
    if(_configuration_adapter)
        set_current_page(_current_page);
}
void ConfigurationFrame::onRectangleChanged(int index, const QRectF& sceneRect) {
    if (!_configuration_adapter || _current_page < 0)
        return;
    
    QSizeF viewSize = _graphics_view->viewport()->size();
    double width = viewSize.width();
    double height = viewSize.height();
    
    // Convertir en coordonnées normalisées (0-1)
    double left = sceneRect.left() / width;
    double top = sceneRect.top() / height;
    double rectWidth = sceneRect.width() / width;
    double rectHeight = sceneRect.height() / height;
    
    // Assurer que les valeurs restent dans [0, 1]
    left = qBound(0.0, left, 1.0);
    top = qBound(0.0, top, 1.0);
    rectWidth = qBound(0.0, rectWidth, 1.0 - left);
    rectHeight = qBound(0.0, rectHeight, 1.0 - top);
    
    // Mettre à jour le modèle
    auto* pageAdapter = _configuration_adapter->page_adapter(_current_page);
    if (index >= 0 && index < pageAdapter->rectangles_count()) {
        auto* rect = pageAdapter->rectangle(index);
        rect->left = left;
        rect->top = top;
        rect->width = rectWidth;
        rect->height = rectHeight;
    }
}
void ConfigurationFrame::contextMenuEvent(QContextMenuEvent* event) {
    QMenu menu(this);
    menu.addAction("New Graph...", this, [this, event](){
        QSizeF size(width(), height());
        QPointF point = event->pos();
        double x = point.x() / size.width();
        double y = point.y() / size.height();
        _configuration_adapter->page_adapter(_current_page)->add_rectangle(x, y, 0.3, 0.3);
    });
    menu.exec(event->globalPos());
}