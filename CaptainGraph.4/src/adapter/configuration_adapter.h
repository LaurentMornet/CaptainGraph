#pragma once
#include <QObject>
#include "../common/configuration.h"
#include "../adapter/page_adapter.h"
#include "../adapter/graph_adapter.h"

class ConfigurationAdapter: public QObject {
    Q_OBJECT
private:
    Configuration*                      _configuration;
    QMap<unsigned short, GraphAdapter*> _graphs_adapters;
    QList<PageAdapter*>                 _pages_adapters;
public:
    explicit ConfigurationAdapter(Configuration* configuration, QObject* parent=nullptr);
    virtual ~ConfigurationAdapter() = default;
    void set_label(const QString& label);

    void new_graph();
    GraphAdapter* graph_adapter(const short& i);

    void add_page();
    short pages_count() const;
    PageAdapter* page_adapter(const short& ipage);
};