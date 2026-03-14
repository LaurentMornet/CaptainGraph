#pragma once
#include <QObject>
#include "../common/graph.h"

/**
 * Liste des propriétés à adapter
 * Format : X(nom, typeQt, typeStd)
 */
#define GRAPH_PROPERTIES \
    X(id, QString, std::string) \
    X(title, QString, std::string) \
    X(x_label, QString, std::string) \
    X(y_label, QString, std::string) \
    X(z_label, QString, std::string) \
    X(grid, bool, bool) \
    X(axis_equal, bool, bool)

class GraphAdapter : public QObject {
    Q_OBJECT
    Graph* _graph;

public:
    explicit GraphAdapter(Graph* graph, QObject* parent = nullptr);
    Graph* graph() const;

#define X(name, qttype, cpptype) \
    Q_PROPERTY(qttype name READ name WRITE set_##name NOTIFY name##_changed) \
    qttype name() const; \
    void set_##name(const qttype& val); \
    Q_SIGNAL void name##_changed(qttype);
    GRAPH_PROPERTIES
#undef X

private:
    // --- Conversion helpers spécialisés ---
    static QString _toQt(const std::string& s);
    static std::string _toCpp(const QString& s);

    static bool _toQt(bool b);
    static bool _toCpp(bool b);

    static bool _equal(const QString& a, const QString& b);
    static bool _equal(bool a, bool b);
};
