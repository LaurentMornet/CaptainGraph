#pragma once

#include <QAction>
#include <QList>
#include <QWidget>
#include <QToolBar>
#include <QWebEngineView>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QResizeEvent>
#include <QGraphicsScene>

#include "../adapter/configuration_adapter.h"
#include "resizable_snap_rect_item.h"

class MainWindow;
class ConfigurationFrame;

class GridGraphicsScene : public QGraphicsScene {
    Q_OBJECT
private:
    ConfigurationFrame* _configuration_frame;
    bool _visible;
protected:
    virtual void drawBackground(QPainter* painter, const QRectF& rect) override;
    
public:
    explicit GridGraphicsScene(ConfigurationFrame* parent);
    void set_grid_visible(const bool& visible);
    void update_scene();
};

class ConfigurationFrame: public QWidget {
    Q_OBJECT
private:
    ConfigurationAdapter*   _configuration_adapter  = nullptr;
    QToolBar*               _toolbar                = nullptr;
    QList<QAction*>         _actions;
    QWebEngineView*         _webview                = nullptr;
    QGraphicsView*          _graphics_view          = nullptr;
    GridGraphicsScene*      _graphics_scene         = nullptr;
    QList<QRectF>           _rectangles;
    short                   _current_page;
    double                  _grid_step;
protected:
    virtual void            resizeEvent(QResizeEvent* event) override;
    virtual void            contextMenuEvent(QContextMenuEvent* event) override;
public:
    explicit ConfigurationFrame(MainWindow* main_window);
    void set_configuration_adapter(ConfigurationAdapter* conf_adapter);
    void set_current_page(const short& ipage);
    void set_grid_step(const double& grid_step);
    double grid_step() const;
private slots:
    void onRectangleChanged(int index, const QRectF& sceneRect);
};