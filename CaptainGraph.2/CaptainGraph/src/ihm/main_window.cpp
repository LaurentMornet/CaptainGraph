#include "main_window.h"
#include "ihm_tools.h"
#include <iostream>

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent) {
    _graphs = {
        Graph(),
        Graph()
    };

    _view_menu = new QMenu(QStringLiteral("&View"), this);

    UndoManager::get_instance(this);

    QToolBar * toolBar = new QToolBar(this);
    addToolBar(toolBar);
    auto * action_undo = toolBar->addAction(IhmTools::load_colored_icon(":/resources/appicons/undo.svg"), "Undo");
    auto * action_redo = toolBar->addAction(IhmTools::load_colored_icon(":/resources/appicons/redo.svg"), "Redo");
    auto * action_test = toolBar->addAction("Test");
    connect(UndoManager::get_instance(this)->undo_stack(), &QUndoStack::canUndoChanged, action_undo, &QAction::setEnabled);
    connect(UndoManager::get_instance(this)->undo_stack(), &QUndoStack::canRedoChanged, action_redo, &QAction::setEnabled);
    connect(action_undo, &QAction::triggered, UndoManager::get_instance(this)->undo_stack(), &QUndoStack::undo);
    connect(action_redo, &QAction::triggered, UndoManager::get_instance(this)->undo_stack(), &QUndoStack::redo);
    connect(action_test, &QAction::triggered, this, &MainWindow::_cb_test_action);
    action_undo->setShortcut(QKeySequence::Undo);
    action_redo->setShortcut(QKeySequence::Redo);
    action_undo->setShortcutContext(Qt::ApplicationShortcut);
    action_redo->setShortcutContext(Qt::ApplicationShortcut);

    menuBar()->setNativeMenuBar(true);
    menuBar()->addMenu(_view_menu);

    _add_dock(Qt::RightDockWidgetArea, 
                QStringLiteral("Graph config"), 
                GraphConfigFrame::get_instance(this));
    _add_dock(Qt::RightDockWidgetArea, 
                QStringLiteral("Plot config"), 
                PlotConfigFrame::get_instance(this));
    _add_dock(Qt::LeftDockWidgetArea,
                QStringLiteral("History"),
                new QUndoView(UndoManager::get_instance()->undo_stack()), true, false);


    test_menu = menuBar()->addMenu("Test");
    test_action = test_menu->addAction("Change graph");
    connect(test_action, &QAction::triggered, this, &MainWindow::_cb_test_action);

    setCentralWidget(new QFrame(this));
    resize(1500, 1000);
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

void MainWindow::_cb_test_action() {
    _current_graph++;
    if(_current_graph >= _graphs.size())
        _current_graph = 0;
    GraphConfigFrame::get_instance()->set_graph(&_graphs[_current_graph]);
}

int main(int argc, char ** argv) {
    Plot::load_base_plots_registry();
	QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("fusion"));
	MainWindow w;
    Plot::PLOT_NUMBER = 0;
	w.show();
	return a.exec();
}
