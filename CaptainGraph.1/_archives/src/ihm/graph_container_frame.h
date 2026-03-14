#pragma once

#include <iostream>

#include <QFrame>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QMenu>
#include <QToolBar>
#include <QToolButton>
#include <QStyle>
#include <QTimer>

#include "graph_frame.h"

class GraphContainerFrame : public QFrame {
    Q_OBJECT
private:
    static inline int MIN_CHILD = 48;
    GraphFrame * _content_frame;
    QAction * _act_merge_left, * _act_merge_right, * _act_merge_up, * _act_merge_down;
    // split
    void _split(Qt::Orientation);
    void _restore_sizes_after_insert_same_orientation(
        QSplitter * parent,
        QList<int> prev_sizes,
        int idx);
    void _restore_sizes_after_nested_insert(
        QSplitter * parent,
        QSplitter * new_splitter,
        QList<int> prev_sizes,
        int idx);
    // merge
    void _update_merge_actions();
    void _merge(const QString & direction);
    void _delete_widget(QWidget * widget);
public:
    explicit GraphContainerFrame(QWidget * parent = nullptr);
    ~GraphContainerFrame() override = default;
    void update_graph();
};