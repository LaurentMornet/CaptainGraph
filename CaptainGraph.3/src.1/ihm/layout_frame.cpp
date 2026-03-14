#include "layout_frame.h"
#include "ihm_tools.h"

void WebView::resizeEvent(QResizeEvent *event) {
    QWebEngineView::resizeEvent(event);
    emit resized();
}
/**
 * Layout Frame implementation
 */
QSize LayoutFrame::sizeHint() const { 
    return {200, 150};
}
LayoutFrame::LayoutFrame(MainWindow* parent): QFrame(parent), _main_window(parent) {
    auto* main_layout = new QVBoxLayout(this);
    setLayout(main_layout);
    _main_frame = new QFrame(this);
    main_layout->addWidget(_main_frame);
    auto* layout = new QVBoxLayout(_main_frame);
    layout->setContentsMargins(0, 0, 0, 0);
    _main_frame->setLayout(layout);
    _webview = new WebView(_main_frame);
    _webview->setHtml("<!doctype html><html><head><style></style></head><body></body></html>");
    _overlay = new SplitOverlay(_main_frame, this);

    layout->addWidget(_webview);

    _overlay->setParent(_main_frame);
    _overlay->raise();
    _overlay->show();

    connect(_webview, &WebView::resized, this, [=] { 
        _overlay->resize(_webview->size()); 
        _webview->page()->runJavaScript(R"(
            document.querySelectorAll('.plot').forEach(
                div => {
                    Plotly.Plots.resize(div);
                });)"
        );
    });

    // connect(_overlay, &SplitOverlay::split_occured, this, [=] {
    //     _overlay->resize(_webview->size());
    // });

    _main_frame->setVisible(false);
}
void LayoutFrame::set_layout_adapter(LayoutAdapter * layout_adapter) {
    _layout_adapter = layout_adapter;
    if(!layout_adapter) {
        _main_frame->setVisible(false);
        _overlay->setParent(nullptr);
        return;
    }
    _main_frame->setVisible(true);
    _overlay->setParent(_main_frame);
    _overlay->show();
    _overlay->raise();
    _overlay->set_layout_adapter(layout_adapter);
    _overlay->rebuild();
    rebuild_plotly();
    _main_frame->repaint();
}
void LayoutFrame::rebuild_plotly() {
    _webview->setHtml(_layout_adapter->to_plotly_html());
}