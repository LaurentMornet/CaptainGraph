#pragma once

#include <QApplication>
#include <QStyleFactory>
#include <QAction>
#include <QDockWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QUndoView>

#include "../common/plot.h"
#include "undo_manager.h"
#include "graph_config_frame.h"
#include "plot_config_frame.h"

class MainWindow: public QMainWindow {
    Q_OBJECT
private:
    QMenu			* _view_menu;
    unsigned int _current_graph = 100;
    std::vector<Graph> _graphs;
    QMenu * test_menu;
    QAction * test_action;
    void _add_dock(Qt::DockWidgetArea where, const QString &label, QWidget * widget, bool visible=true, bool floating=false);
private slots:
    void _cb_test_action();
public:
    explicit MainWindow(QWidget * parent = nullptr);
    virtual ~MainWindow() override = default;
};

int main(int argc, char ** argv);