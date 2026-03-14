#include "auto_label_widget.h"

AutoLabelWidget::AutoLabelWidget(QWidget * parent, QString initial, bool initial_check)
    : QFrame(parent) {
    auto * layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);
    _noauto_value = initial;
    _auto_value = "";
    _edt_label = new QLineEdit(initial, this);
    _edt_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _chk_auto = new QCheckBox(); _chk_auto->setTristate(false); _chk_auto->setChecked(initial_check);
    _chk_auto->setToolTip("auto");
    connect(_chk_auto, &QCheckBox::stateChanged, this, &AutoLabelWidget::_cb_auto_checked);
    connect(_edt_label, &QLineEdit::textChanged, this, &AutoLabelWidget::_cb_txt_changed);
    layout->addWidget(_edt_label);
    layout->addWidget(_chk_auto);
    _cb_auto_checked();
}

QString AutoLabelWidget::text() {
    return _edt_label->text();
}
void AutoLabelWidget::set_noauto_text(QString text) {
    _noauto_value = text;
    if(!_chk_auto->isChecked())
        _edt_label->setText(text);
}
void AutoLabelWidget::set_auto_text(QString text) {
    _auto_value = text;
    if(_chk_auto->isChecked())
        _edt_label->setText(text);
}
void AutoLabelWidget::_cb_auto_checked() {
    if(_chk_auto->isChecked()) {
        emit signal_auto_checked();
        _edt_label->setEnabled(false);
        _edt_label->setText(_auto_value);    
    }
    else {
        _edt_label->setEnabled(true);
        _edt_label->setText(_noauto_value);
    }
}
void AutoLabelWidget::_cb_txt_changed(QString value) {
    if(!_chk_auto->isChecked())
        _noauto_value = value;
}