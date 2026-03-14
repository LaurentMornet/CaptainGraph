#include "main_window.h"
#include "ihm_tools.h"
#include <iostream>

MainWindow::MainWindow(Manager* manager, QWidget * parent)
    : QMainWindow(parent), _manager(manager) {

    menuBar()->setNativeMenuBar(false);

    _view_menu = new QMenu(QStringLiteral("&View"), this);
    menuBar()->addMenu(_view_menu);

    auto * _test_menu = new QMenu(QStringLiteral("&Test"), this);
    menuBar()->addMenu(_test_menu);
    auto * test_action_0 = _test_menu->addAction("Test 0");
    connect(test_action_0, &QAction::triggered, this, &MainWindow::_cb_test_action_0);

    _layout_frame = new LayoutFrame(this);
    _graph_frame = new GraphFrame(this);
    _plot_frame = new PlotFrame(this);

    _add_dock(Qt::RightDockWidgetArea, QStringLiteral("Layout"), _layout_frame);
    _add_dock(Qt::RightDockWidgetArea, QStringLiteral("Graph"), _graph_frame);
    _add_dock(Qt::RightDockWidgetArea, QStringLiteral("Plot"), _plot_frame);

    setCentralWidget(new QFrame(this));
    resize(1400, 900);
    setWindowTitle("Captain'Graph");
}

void MainWindow::_add_dock(Qt::DockWidgetArea where,
                            const QString &label, 
                            QWidget * widget,
                            bool visible,
                            bool floating) {
    auto * dock = new QDockWidget(label, this);
    dock->setWidget(widget);
    addDockWidget(where, dock);
    _view_menu->addAction(dock->toggleViewAction());
    dock->setVisible(visible);
    dock->setFloating(floating);
}

LayoutFrame* MainWindow::layout_frame() { return _layout_frame; }
GraphFrame* MainWindow::graph_frame() { return _graph_frame; }
PlotFrame* MainWindow::plot_frame() { return _plot_frame; }

void MainWindow::_cb_test_action_0() {
    _layout_frame->set_layout_adapter(new LayoutAdapter(&_manager->configuration().layout(0)));
    // _layout_frame->set_layout_adapter(new LayoutAdapter(&Manager::get_instance().configuration().layout(0), this));
}