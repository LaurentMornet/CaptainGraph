#pragma once

#include <QFrame>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>

#include "../adapter/layout_adapter.h"
#include "../common/manager.h"
#include "main_window.h"
#include "graph_frame.h"

class MainWindow;
class MiniLayoutFrame;

class MiniLayoutRenderWidget: public QWidget {
    Q_OBJECT
private:
    MiniLayoutFrame* _mini_layout_frame;
    QString _current_id;
    virtual QSize sizeHint() const override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
public:
    explicit MiniLayoutRenderWidget(MiniLayoutFrame* parent=nullptr);
    virtual ~MiniLayoutRenderWidget()=default;
signals:
    void layout_selected(const QString&);
};

class MiniLayoutFrame: public QFrame {
    Q_OBJECT
private:
    friend class MiniLayoutRenderWidget;
    MainWindow* _main_window;
    QSize sizeHint() const override;
    LayoutAdapter* _layout_adapter=nullptr;
    QFrame* _main_frame=nullptr;
    MiniLayoutRenderWidget* _render_widget=nullptr;
private slots:
    void _cb_layout_selected(const QString&);
public slots:
    void _cb_weights_changed();
public:
    MiniLayoutFrame(MainWindow* parent);
    virtual ~MiniLayoutFrame()=default;
    MiniLayoutFrame(const MiniLayoutFrame&)=delete;
    MiniLayoutFrame& operator=(const MiniLayoutFrame&)=delete;
    void set_layout_adapter(LayoutAdapter* layout_adapter);
    LayoutAdapter* layout_adapter() const;
};