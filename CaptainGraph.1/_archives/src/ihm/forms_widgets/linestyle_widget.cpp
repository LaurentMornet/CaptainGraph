#include "linestyle_widget.h"

LineStyleWidget::LineStyleWidget(QWidget * parent): QComboBox(parent) {
    setStyle(QStyleFactory::create("Fusion"));
    setIconSize(QSize(100, 30));
    setFixedHeight(30);
    addItem("none");
    addItem(QIcon(":/resources/metaplots/styles/linestyles/solid.png"), "solid");
    addItem(QIcon(":/resources/metaplots/styles/linestyles/dot.png"), "dot");
    addItem(QIcon(":/resources/metaplots/styles/linestyles/dashdot.png"), "dash dot");
}

void LineStyleWidget::_cb_selection_changed() {

}