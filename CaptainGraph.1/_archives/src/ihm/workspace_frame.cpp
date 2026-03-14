#include "workspace_frame.h"

WorkspaceFrame::WorkspaceFrame(QWidget * parent)
    : QFrame(parent) {
        
    auto *layout = new QVBoxLayout(this);
    setLayout(layout);

    _tab_widget = new QTabWidget(this);
    layout->addWidget(_tab_widget);

    auto *cont = new QFrame(_tab_widget);
    auto *lcont = new QVBoxLayout(cont);
    lcont->setContentsMargins(0, 0, 0, 0);
    auto * splitter = new QSplitter(cont);
    splitter->addWidget(new GraphContainerFrame(splitter));
    lcont->addWidget(splitter);
    _tab_widget->addTab(cont, QStringLiteral("MOC"));
}
