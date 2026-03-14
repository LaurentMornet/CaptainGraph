#pragma once

#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QResizeEvent>
#include <QToolBar>
#include <QPushButton>

#include "../adapter/layout_adapter.h"

class LayoutPanel : public QFrame {
    Q_OBJECT
public:
    explicit LayoutPanel(LayoutAdapter* layout_adapter, const QString& layoutId, QWidget* parent = nullptr);
signals:
    void request_split(const QString& layout_id, LayoutType type);
    void request_merge(const QString& layout_id, MergeDirection direction);
    void requestMerge(const QString& layoutId);
private:
    LayoutAdapter* _layout_adapter;
    QString _id;
    QToolBar* _toolbar;
    QPushButton* _btnMenu;
};
