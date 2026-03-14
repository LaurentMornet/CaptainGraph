#include "graph_config_frame.h"

GraphConfigFrame::GraphConfigFrame(QWidget * parent)
    : QFrame(parent), _managed_metagraph(nullptr) {
    _setup();
}

void GraphConfigFrame::_setup() {
    auto * layout = new QFormLayout(this);
    layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    _edt_title = new QLineEdit("(no graph selected)", this);
    _edt_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setLayout(layout);
    layout->addRow("Title", _edt_title);
    connect(this, &GraphConfigFrame::selected, this, &GraphConfigFrame::slot_selected);
}

void GraphConfigFrame::_set_metagraph(MetaGraph * metagraph) {
    _managed_metagraph = metagraph;
    if(_managed_metagraph) {
        _edt_title->setText(QString(metagraph->title().c_str()));
    } else {
        _edt_title->setText("(no graph selected)");
    }
}

void GraphConfigFrame::slot_selected(MetaGraph * metagraph) {
    _set_metagraph(metagraph);
}

MetaGraph * GraphConfigFrame::metagraph() {
    return _managed_metagraph;
}