#include "graph_adapter.h"

GraphAdapter::GraphAdapter(Graph* graph, QObject* parent)
    : QObject(parent), _graph(graph) {}

Graph* GraphAdapter::graph() const { return _graph; }

// --- Génération automatique des propriétés ---
#define X(name, qttype, cpptype) \
    qttype GraphAdapter::name() const { \
        return _toQt(_graph->name()); \
    } \
    void GraphAdapter::set_##name(const qttype& val) { \
        if (_equal(val, _toQt(_graph->name()))) return; \
        _graph->set_##name(_toCpp(val)); \
        emit name##_changed(val); \
    }
GRAPH_PROPERTIES
#undef X

// --- Conversion helpers ---
// QString <-> std::string
QString GraphAdapter::_toQt(const std::string& s) { return QString::fromStdString(s); }
std::string GraphAdapter::_toCpp(const QString& s) { return s.toStdString(); }

// bool <-> bool (identité)
bool GraphAdapter::_toQt(bool b) { return b; }
bool GraphAdapter::_toCpp(bool b) { return b; }

// Comparaisons
bool GraphAdapter::_equal(const QString& a, const QString& b) { return a == b; }
bool GraphAdapter::_equal(bool a, bool b) { return a == b; }
