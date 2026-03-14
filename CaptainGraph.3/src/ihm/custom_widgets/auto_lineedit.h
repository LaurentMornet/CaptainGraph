#pragma once

#include <QLineEdit>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QFrame>

class AutoLineEdit: public QFrame {
    Q_OBJECT
private:
    QLineEdit* _edt_text;
    QCheckBox* _cbx_auto;
    bool _checked;
    QString _user_text = "";
    QString _auto_text = "(auto)";
private slots:
    void _cb_checked();
    void _cb_text_changed();
public:
    explicit AutoLineEdit(QString user_text, bool initial_check, QWidget* parent=nullptr);
    virtual ~AutoLineEdit()=default;
    void set_user_text(const QString& user_text);
    void set_auto_text(const QString& auto_text);
    QString text() const;
signals:
    void auto_checked(bool);
    void editing_finished(const QString&);
};