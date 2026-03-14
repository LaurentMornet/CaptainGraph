#pragma once

#include <QFrame>
#include <QTabWidget>
#include <QVBoxLayout>
#include "./graph_container_frame.h"

class WorkspaceFrame : public QFrame {
    Q_OBJECT
public:
    explicit WorkspaceFrame(QWidget * parent = nullptr);
    ~WorkspaceFrame() override = default;

private:
    QTabWidget* _tab_widget{nullptr};
};