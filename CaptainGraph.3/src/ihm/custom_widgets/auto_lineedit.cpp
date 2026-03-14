#include "auto_lineedit.h"

AutoLineEdit::AutoLineEdit(QString user_text, bool initial_check, QWidget* parent): 
    QFrame(parent),
    _checked(initial_check),
    _user_text(user_text),
    _auto_text("(auto)") {
    auto* layout = new QHBoxLayout(this);
    _edt_text = new QLineEdit(this);
    _cbx_auto = new QCheckBox(this);
    _cbx_auto->setTristate(false);
    _cbx_auto->setCheckState(_checked ? Qt::Checked: Qt::Unchecked);
    layout->addWidget(_edt_text);
    layout->addWidget(_cbx_auto);
    layout->setContentsMargins(0, 0, 0, 0);
    _user_text = user_text;
    _edt_text->setEnabled(!_checked);
    _edt_text->setText(_checked ? _auto_text : _user_text);
    connect(_cbx_auto, &QCheckBox::stateChanged, this, &AutoLineEdit::_cb_checked);
    connect(_edt_text, &QLineEdit::editingFinished, this, &AutoLineEdit::_cb_text_changed);
}
void AutoLineEdit::set_user_text(const QString& user_text) {
    _user_text = user_text;
}
void AutoLineEdit::set_auto_text(const QString& auto_text) {
    _auto_text = auto_text;
}
QString AutoLineEdit::text() const {
    return _edt_text->text();
}
void AutoLineEdit::_cb_checked() {
    _checked = _cbx_auto->checkState() == Qt::Checked;
    _edt_text->setEnabled(!_checked);
    _edt_text->setText(_checked ? _auto_text : _user_text);
}
void AutoLineEdit::_cb_text_changed() {
    _user_text = _edt_text->text();
}