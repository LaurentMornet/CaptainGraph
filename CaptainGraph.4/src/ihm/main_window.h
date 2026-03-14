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
#include "../adapter/manager_adapter.h"
#include "graph_frame.h"
#include "plot_frame.h"
#include "configuration_frame.h"

class GraphFrame;
class PlotFrame;

class MainWindow: public QMainWindow {
    Q_OBJECT
private:
    ManagerAdapter          _manager_adapter;
    QMenu*			        _view_menu;
    GraphFrame*             _graph_frame;
    PlotFrame*              _plot_frame;
    ConfigurationFrame*     _workspace;
    void _add_dock(Qt::DockWidgetArea where, 
        const QString &label, 
        QWidget * widget, 
        bool visible=true, 
        bool floating=false);
private slots:
    void _cb_test_action_0();
    // void _cb_rename_tab(int tab_index);
    void _cb_configuration_loaded();
public:
    explicit MainWindow(Manager* manager, QWidget * parent = nullptr);
    virtual ~MainWindow() override = default;
    // accès aux frames
    GraphFrame* graph_frame();
    PlotFrame* plot_frame();
};

int main(int argc, char ** argv);