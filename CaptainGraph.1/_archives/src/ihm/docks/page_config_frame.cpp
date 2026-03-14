#include "page_config_frame.h"

PageConfigFrame::PageConfigFrame(QWidget * parent)
    : QFrame(parent) {
    _setup();
}

void PageConfigFrame::_setup() {
    auto * layout = new QVBoxLayout(this);
    auto * label = new QLabel("Page config frame contents");
    setLayout(layout);
    layout->addWidget(label);
}
