#include "manager_adapter.h"

ManagerAdapter::ManagerAdapter(Manager* manager):
    _manager(manager) {
}
void ManagerAdapter::load_configuration() {
    if(!_configuration_adapter)
        _configuration_adapter->deleteLater();
    _configuration_adapter = new ConfigurationAdapter(&_manager->configuration());

    _configuration_adapter->set_label("Demo Configuration");
    _configuration_adapter->add_layout();
    _configuration_adapter->set_layout_title(0, "First layout");
    // _configuration.sublayout(0, "A").split(LayoutType::Horizontal);
    // _configuration.sublayout(0, "B").split(LayoutType::Vertical);
    // _configuration.sublayout(0, "A").set_weight(0.2);
    _configuration_adapter->add_layout();
    _configuration_adapter->set_layout_title(1, "Second layout");

    emit configuration_loaded();
}
ConfigurationAdapter* ManagerAdapter::configuration_adapter() {
    return _configuration_adapter;
}