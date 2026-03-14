#include "fs_browser_frame.h"
#include <iostream>

FsBrowserFrame::FsBrowserFrame(QWidget *parent)
    : QFrame(parent) {
    _setup();
}

void FsBrowserFrame::_setup() {
    _model = new QFileSystemModel(this);
    _model->setRootPath(QStringLiteral("/home/laurent/host"));

    _tree_view = new QTreeView(this);
    _tree_view->setModel(_model);
    _tree_view->setRootIndex(_model->index(QStringLiteral("/home/laurent/host")));

    auto *header = _tree_view->header();
    header->setSectionResizeMode(0, QHeaderView::Stretch);
    header->setStretchLastSection(false);
    header->setSectionResizeMode(1, QHeaderView::Interactive);

    _tree_view->hideColumn(1);
    _tree_view->hideColumn(2);

    auto *layout = new QVBoxLayout(this);
    auto *btn = new QPushButton(QStringLiteral("Click Me"), this);

    layout->addWidget(_tree_view);
    layout->addWidget(btn);
    this->setLayout(layout);

    connect(btn, &QPushButton::clicked, this, &FsBrowserFrame::_cb_say_hello);
}

void FsBrowserFrame::_cb_say_hello() {
    AppContext::getInstance()->check();
    std::cout << "So, I say Hello !" << std::endl;
}
