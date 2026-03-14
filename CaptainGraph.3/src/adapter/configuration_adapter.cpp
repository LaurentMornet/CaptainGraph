#include "configuration_adapter.h"

ConfigurationAdapter::ConfigurationAdapter(Configuration* configuration, QObject* parent):
    QObject(parent), _configuration(configuration) {}
void ConfigurationAdapter::set_label(const QString& label) {
    _configuration->set_label(label.toStdString());
}
unsigned short ConfigurationAdapter::layouts_count() const { 
    return _configuration ? _configuration->layouts_count(): 0; 
}
QString ConfigurationAdapter::layout_title(unsigned short idx) const { 
    return QString::fromStdString(_configuration->layout_title(idx)); 
}
void ConfigurationAdapter::set_layout_title(const unsigned short idx, const QString& new_title) {
    _configuration->set_layout_title(idx, new_title.toStdString());
}
LayoutAdapter* ConfigurationAdapter::add_layout() {
    _layout_adapters.append(new LayoutAdapter(&_configuration->add_layout()));
    return layout_adapter(_layout_adapters.size() - 1);
}
LayoutAdapter* ConfigurationAdapter::layout_adapter(unsigned short idx) { 
    if(idx >= 0 && idx < _layout_adapters.size())
        return _layout_adapters[idx];
    return nullptr;
}