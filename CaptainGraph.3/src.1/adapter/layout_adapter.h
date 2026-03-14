#pragma once

#include <QObject>
#include <QMap>
#include <QList>
#include <QString>

#include "../common/layout.h"

class LayoutAdapter: public QObject {
    Q_OBJECT
private:
    Layout* _layout; // pointeur non-possédant vers la racine Layout
public:
    explicit LayoutAdapter(Layout* layout, QObject* parent = nullptr);

    QMap<QString, QList<double>> nodes_bounds() const;
    int get_layout_index() const;
    Graph* graph(const QString& id);

    // accès direct à la racine
    Layout* layout_ptr() const;

    // Recherche par id (QString -> std::string)
    Layout* find(const QString& id) const;

    QList<QString> siblings(const QString& id);

    void set_weight(const QString& id, double weight);
    double weight(const QString& id) const;

    void split(const QString& id, LayoutType type);
    bool can_merge(const QString& id, MergeDirection direction);
    void merge(const QString& id, MergeDirection direction);

    QString to_plotly_html();

signals:
    void weights_changed();
    void split_occured();
    void merge_occured();
};
