#include "configuration_adapter.h"

ConfigurationAdapter::ConfigurationAdapter(Configuration* configuration, QObject* parent):
    QObject(parent), _configuration(configuration) {
}
void ConfigurationAdapter::set_label(const QString& label) {
    _configuration->set_label(label.toStdString());
}
void ConfigurationAdapter::new_graph() {
    short i = _configuration->new_graph();
    _graphs_adapters.insert(i, new GraphAdapter(_configuration->graph(i), this));
}
GraphAdapter* ConfigurationAdapter::graph_adapter(const short& i) {
    return _graphs_adapters[i];
}
void ConfigurationAdapter::add_page() {
    _configuration->add_page();
    _pages_adapters.append(new PageAdapter(_configuration->page(_configuration->pages_count() - 1), this));
}
short ConfigurationAdapter::pages_count() const {
    return _pages_adapters.size();
}
PageAdapter* ConfigurationAdapter::page_adapter(const short& ipage) {
    return _pages_adapters[ipage];
}