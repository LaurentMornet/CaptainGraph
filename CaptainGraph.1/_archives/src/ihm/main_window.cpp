#include "main_window.h"
#include "workspace_frame.h"

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent) {
    _workspace_frame = new WorkspaceFrame(this);
    _view_menu = new QMenu(QStringLiteral("&View"), this);
    menuBar()->setNativeMenuBar(true);
    menuBar()->addMenu(_view_menu);
    _add_dock(Qt::LeftDockWidgetArea,
                QStringLiteral("FS Browser"), 
                FsBrowserFrame::getInstance(this));
    _add_dock(Qt::LeftDockWidgetArea, 
                QStringLiteral("Page config"), 
                PageConfigFrame::getInstance(this));
    _add_dock(Qt::RightDockWidgetArea, 
                QStringLiteral("Graph config"), 
                GraphConfigFrame::getInstance(this));
    _add_dock(Qt::RightDockWidgetArea, 
                QStringLiteral("Plot config"), 
                PlotConfigFrame::getInstance(this));
    setCentralWidget(_workspace_frame);
    resize(1026, 768);
    setWindowTitle("Captain'Graph");
}

void MainWindow::_add_dock(Qt::DockWidgetArea where,
                            const QString &label, 
                            QWidget * widget) {
    auto * dock = new QDockWidget(label, this);
    dock->setWidget(widget);
    addDockWidget(where, dock);
    _view_menu->addAction(dock->toggleViewAction());
}