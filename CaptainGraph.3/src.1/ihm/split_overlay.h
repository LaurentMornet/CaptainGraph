#pragma once

#include <QWidget>
#include <QPointer>
#include <QMap>
#include <QList>
#include <QSplitter>

#include "../adapter/layout_adapter.h"
// #include "layout_frame.h"
class LayoutFrame;

class SplitOverlay : public QWidget {
    Q_OBJECT
public:
    explicit SplitOverlay(QWidget* parent, LayoutFrame* layout_frame);
    void set_layout_adapter(LayoutAdapter* layout_adapter);
    void rebuild();
protected:
    void resizeEvent(QResizeEvent* e) override;
private slots:
    void _cb_splitter_moved(int pos, int index);
private:
    LayoutFrame* _layout_frame;
    QMap<QSplitter*, QList<QString>>  _splitter_map;
    QMap<QSplitter*, QList<QWidget*>> _splitter_mapw;
    QWidget* build_widget_from_layout(Layout* layout);
    void clear_root_widget();
    QPointer<LayoutAdapter> _layout_adapter;
    QWidget* _rootWidget = nullptr;
    bool _rebuild_pending = false;
// signals:
//     void split_occured();
};
