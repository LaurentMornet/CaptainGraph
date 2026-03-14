#pragma once

#include <QDockWidget>
#include <QFrame>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>

#include "../common/manager.h"
#include "../common/layout.h"
#include "layout_frame.h"
#include "graph_frame.h"
#include "plot_frame.h"

class LayoutFrame;
class GraphFrame;
class PlotFrame;

class MainWindow: public QMainWindow {
    Q_OBJECT
private:
    Manager*        _manager;
    QMenu*			_view_menu;
    LayoutFrame*    _layout_frame;
    GraphFrame*     _graph_frame;
    PlotFrame*      _plot_frame;
    void _add_dock(Qt::DockWidgetArea where, const QString &label, QWidget * widget, bool visible=true, bool floating=false);
private slots:
    void _cb_test_action_0();
public:
    explicit MainWindow(Manager* manager, QWidget * parent = nullptr);
    virtual ~MainWindow() override = default;
    // accès aux frames
    LayoutFrame* layout_frame();
    GraphFrame* graph_frame();
    PlotFrame* plot_frame();
};

int main(int argc, char ** argv);