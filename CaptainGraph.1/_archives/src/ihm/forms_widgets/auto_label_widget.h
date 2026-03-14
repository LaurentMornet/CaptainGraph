#pragma once
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QHBoxLayout>

class AutoLabelWidget: public QFrame {
    Q_OBJECT
signals:
    void signal_auto_checked();
private:
    QString _noauto_value;
    QString _auto_value;
    QLineEdit * _edt_label;
    QCheckBox * _chk_auto;
public:
    explicit AutoLabelWidget(QWidget * parent=nullptr, QString initial="", bool initial_check=true);
    virtual ~AutoLabelWidget() = default;
    QString text();
    void set_noauto_text(QString text);
    void set_auto_text(QString text);
private slots:
    void _cb_auto_checked();
    void _cb_txt_changed(QString value);
};