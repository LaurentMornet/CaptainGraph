#pragma once

#include <QDockWidget>
#include <QFrame>
#include <QList>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QTabWidget>
#include <QPushButton>

#include "../common/manager.h"
#include "../common/layout.h"
#include "../adapter/manager_adapter.h"
#include "mini_layout_frame.h"
#include "layout_frame.h"
#include "graph_frame.h"
#include "plot_frame.h"

class MiniLayoutFrame;
class LayoutFrame;
class GraphFrame;
class PlotFrame;

class MainWindow: public QMainWindow {
    Q_OBJECT
private:
    ManagerAdapter          _manager_adapter;
    LayoutAdapter*          _active_layout_adapter;
    QMenu*			        _view_menu;
    MiniLayoutFrame*        _mini_layout_frame;
    GraphFrame*             _graph_frame;
    PlotFrame*              _plot_frame;
    QTabWidget*             _workspace;
    QList<LayoutFrame*>     _layouts_frames;
    void _add_dock(Qt::DockWidgetArea where, 
        const QString &label, 
        QWidget * widget, 
        bool visible=true, 
        bool floating=false);
private slots:
    void _cb_test_action_0();
    void _cb_test_action_1();
    void _cb_rename_tab(int tab_index);
    void _cb_configuration_loaded();
    void _cb_active_layout_changed(int tab_index);
    void _cb_layout_weights_changed();
public:
    explicit MainWindow(Manager* manager, QWidget * parent = nullptr);
    virtual ~MainWindow() override = default;
    // accès aux frames
    MiniLayoutFrame* mini_layout_frame();
    GraphFrame* graph_frame();
    PlotFrame* plot_frame();
};

int main(int argc, char ** argv);