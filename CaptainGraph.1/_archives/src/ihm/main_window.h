#pragma once

#include <iostream>

#include <QMainWindow>
#include <QFrame>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QDockWidget>
#include <QCloseEvent>
#include <QString>

#include "../common/app_context.h"
#include "workspace_frame.h"
#include "./docks/fs_browser_frame.h"
#include "./docks/page_config_frame.h"
#include "./docks/graph_config_frame.h"
#include "./docks/plot_config_frame.h"

class MainWindow : public QMainWindow
{
Q_OBJECT
	WorkspaceFrame		* _workspace_frame;
	QMenu				* _view_menu;
private:
	void _add_dock(Qt::DockWidgetArea where, const QString &label, QWidget * widget);
	void _setup(void);
public:
	explicit MainWindow(QWidget * parent = nullptr);
	~MainWindow() override = default;
};