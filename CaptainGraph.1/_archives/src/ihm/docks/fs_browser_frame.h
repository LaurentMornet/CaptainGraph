#pragma once

#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileSystemModel>
#include <QTreeView>
#include <QHeaderView>

#include "../../common/app_context.h"
#include "../../common/singleton.h"
/**
 * File System Browser
 */
class FsBrowserFrame : public QFrame, public Singleton<FsBrowserFrame> {
    Q_OBJECT
    friend class Singleton<FsBrowserFrame>;
signals:
private:
    QFileSystemModel *_model{nullptr};
    QTreeView *_tree_view{nullptr};
    void _setup();
    explicit FsBrowserFrame(QWidget *parent = nullptr);
    ~FsBrowserFrame() override = default;
private slots:
    void _cb_say_hello();
};