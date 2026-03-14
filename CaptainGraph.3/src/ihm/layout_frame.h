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
class LayoutFrame;

class LayoutRenderWidget: public QWidget {
    Q_OBJECT
private:
    LayoutAdapter* _layout_adapter;
    QString _current_id;
    virtual QSize sizeHint() const override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
public:
    explicit LayoutRenderWidget(QWidget* parent=nullptr);
    virtual ~LayoutRenderWidget()=default;
    void set_layout_adapter(LayoutAdapter* layout_adapter);
signals:
    void layout_selected(const QString&);
};

class LayoutFrame: public QFrame {
    Q_OBJECT
private:
    friend class LayoutRenderWidget;
    MainWindow* _main_window;
    QSize sizeHint() const override;
    LayoutAdapter* _layout_adapter=nullptr;
    QFrame* _main_frame=nullptr;
    LayoutRenderWidget* _render_widget=nullptr;
private slots:
    void _cb_layout_selected(const QString&);
public:
    LayoutFrame(MainWindow* parent);
    virtual ~LayoutFrame()=default;
    LayoutFrame(const LayoutFrame&)=delete;
    LayoutFrame& operator=(const LayoutFrame&)=delete;
    void set_layout_adapter(LayoutAdapter* layout_adapter);
};