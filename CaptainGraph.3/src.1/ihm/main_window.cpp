#include "main_window.h"
#include "ihm_tools.h"
#include <QInputDialog>

MainWindow::MainWindow(Manager* manager, QWidget * parent) : 
    QMainWindow(parent), 
    _manager_adapter(manager),
    _active_layout_adapter(nullptr) {

    menuBar()->setNativeMenuBar(false);

    _view_menu = new QMenu(QStringLiteral("&View"), this);
    menuBar()->addMenu(_view_menu);

    auto * _test_menu = new QMenu(QStringLiteral("&Test"), this);
    menuBar()->addMenu(_test_menu);
    auto * test_action_0 = _test_menu->addAction("Test 0");
    connect(test_action_0, &QAction::triggered, this, &MainWindow::_cb_test_action_0);
    auto * test_action_1 = _test_menu->addAction("Open configuration");
    connect(test_action_1, &QAction::triggered, this, &MainWindow::_cb_test_action_1);

    _mini_layout_frame = new MiniLayoutFrame(this);
    _graph_frame = new GraphFrame(this);
    _plot_frame = new PlotFrame(this);

    _add_dock(Qt::RightDockWidgetArea, QStringLiteral("MiniLayout"), _mini_layout_frame);
    _add_dock(Qt::RightDockWidgetArea, QStringLiteral("Graph"), _graph_frame);
    _add_dock(Qt::RightDockWidgetArea, QStringLiteral("Plot"), _plot_frame);

    _workspace = new QTabWidget(this);
    _workspace->setStyleSheet("QTabWidget::pane {border: 0px solid white;}");
    _workspace->setTabsClosable(true);
    connect(_workspace, &QTabWidget::tabBarDoubleClicked, this, &MainWindow::_cb_rename_tab);
    QPushButton* btn = new QPushButton(IhmTools::load_colored_icon(":/resources/appicons/add.svg"), "", this);
    _workspace->setCornerWidget(btn, Qt::TopRightCorner);

    setCentralWidget(_workspace);
    resize(1400, 900);
    setWindowTitle("Captain'Graph");
    connect(&_manager_adapter, &ManagerAdapter::configuration_loaded, this, &MainWindow::_cb_configuration_loaded);
    connect(_workspace, &QTabWidget::currentChanged, this, &MainWindow::_cb_active_layout_changed);
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
MiniLayoutFrame* MainWindow::mini_layout_frame() { 
    return _mini_layout_frame; 
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
void MainWindow::_cb_test_action_1() {
}
void MainWindow::_cb_rename_tab(int tab_index) {
    if(_manager_adapter.configuration_adapter()) {
        bool ok{};
        QString new_label = QInputDialog::getText(this,
            "Rename layout", "",
            QLineEdit::Normal,
            _manager_adapter.configuration_adapter()->layout_title(tab_index),
            &ok);
        if(ok && !new_label.isEmpty()) {
            _manager_adapter.configuration_adapter()->set_layout_title(tab_index, new_label);
            _workspace->setTabText(tab_index, new_label);
        }
    }
}
void MainWindow::_cb_configuration_loaded() {
    for(unsigned short i=0; i<_layouts_frames.size(); i++)
        _layouts_frames[i]->deleteLater();
    _layouts_frames.clear();
    _workspace->clear();
    if(_manager_adapter.configuration_adapter()) {
        for(unsigned short i=0; i<_manager_adapter.configuration_adapter()->layouts_count(); i++) {
            LayoutFrame* layout_frame = new LayoutFrame(this);
            _workspace->addTab(layout_frame, _manager_adapter.configuration_adapter()->layout_title(i));
            layout_frame->set_layout_adapter(_manager_adapter.configuration_adapter()->layout_adapter(i));
        }
    }
    _cb_active_layout_changed(0);
}
void MainWindow::_cb_active_layout_changed(int tab_index) {
    if(_active_layout_adapter)
        disconnect(_active_layout_adapter, nullptr, nullptr, nullptr);
    _active_layout_adapter = _manager_adapter.configuration_adapter()->layout_adapter(tab_index);
    _mini_layout_frame->set_layout_adapter(_active_layout_adapter);
    if(_active_layout_adapter) {
        connect(_active_layout_adapter, &LayoutAdapter::weights_changed, this, &MainWindow::_cb_layout_weights_changed);
        connect(_active_layout_adapter, &LayoutAdapter::split_occured, this, &MainWindow::_cb_layout_weights_changed);
        connect(_active_layout_adapter, &LayoutAdapter::merge_occured, this, &MainWindow::_cb_configuration_loaded);
    }
}
void MainWindow::_cb_layout_weights_changed() {
    _mini_layout_frame->_cb_weights_changed();
}