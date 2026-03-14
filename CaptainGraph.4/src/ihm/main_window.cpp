#include "main_window.h"
#include "ihm_tools.h"
#include <QInputDialog>

MainWindow::MainWindow(Manager* manager, QWidget * parent) : 
    QMainWindow(parent), 
    _manager_adapter(manager) {

    menuBar()->setNativeMenuBar(false);

    _view_menu = new QMenu(QStringLiteral("&View"), this);
    menuBar()->addMenu(_view_menu);

    auto * _test_menu = new QMenu(QStringLiteral("&Test"), this);
    menuBar()->addMenu(_test_menu);
    _test_menu->addAction("Open configuration", this, &MainWindow::_cb_test_action_0);
    _graph_frame = new GraphFrame(this);
    _plot_frame = new PlotFrame(this);
    _add_dock(Qt::RightDockWidgetArea, QStringLiteral("Graph"), _graph_frame);
    _add_dock(Qt::RightDockWidgetArea, QStringLiteral("Plot"), _plot_frame);
    _workspace = new ConfigurationFrame(this);
    setCentralWidget(_workspace);
    resize(1400, 900);
    setWindowTitle("Captain'Graph");
    connect(&_manager_adapter, &ManagerAdapter::configuration_loaded, this, &MainWindow::_cb_configuration_loaded);
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
GraphFrame* MainWindow::graph_frame() { 
    return _graph_frame;
}
PlotFrame* MainWindow::plot_frame() {
    return _plot_frame; 
}
void MainWindow::_cb_test_action_0() {
    _manager_adapter.load_configuration();
}
void MainWindow::_cb_configuration_loaded() {
    _workspace->set_configuration_adapter(_manager_adapter.configuration_adapter());
}