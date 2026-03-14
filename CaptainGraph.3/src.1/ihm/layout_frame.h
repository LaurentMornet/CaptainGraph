#pragma once

#include <QFrame>
#include <QWebEngineView>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QVBoxLayout>
#include <QSplitter>
#include <QVBoxLayout>
#include <QPushButton>

#include "main_window.h"
#include "split_overlay.h"
class MainWindow;

class WebView: public QWebEngineView {
    Q_OBJECT
public:
    using QWebEngineView::QWebEngineView;
signals:
    void resized();
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
};

class LayoutFrame: public QFrame {
    Q_OBJECT
private:
    friend class MiniLayoutRenderWidget;
    MainWindow* _main_window;
    QSize sizeHint() const override;
    LayoutAdapter* _layout_adapter=nullptr;
    QFrame* _main_frame=nullptr;
    WebView* _webview=nullptr;
    SplitOverlay* _overlay;
public:
    LayoutFrame(MainWindow* parent);
    virtual ~LayoutFrame()=default;
    LayoutFrame(const LayoutFrame&)=delete;
    LayoutFrame& operator=(const LayoutFrame&)=delete;
    void set_layout_adapter(LayoutAdapter* layout_adapter);
    void rebuild_plotly();
};