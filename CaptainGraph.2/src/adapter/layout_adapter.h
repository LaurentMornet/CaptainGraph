#pragma once

#include <QObject>
#include <QMap>
#include <QList>

#include "../common/manager.h"
#include "../common/layout.h"

class LayoutAdapter: public QObject {
    Q_OBJECT
private:
    Layout* _layout;
public:
    LayoutAdapter(Layout* layout, QObject* parent = nullptr);
    QMap<QString, QList<double>> nodes_bounds() const;
    int get_layout_index() const;
    Graph* graph(const QString& id);
};