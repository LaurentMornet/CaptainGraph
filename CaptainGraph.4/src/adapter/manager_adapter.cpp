#include "manager_adapter.h"

ManagerAdapter::ManagerAdapter(Manager* manager):
    _manager(manager) {
}
void ManagerAdapter::load_configuration() {
    if(_configuration_adapter)
        _configuration_adapter->deleteLater();
    _configuration_adapter = new ConfigurationAdapter(&_manager->configuration());
    _configuration_adapter->set_label("Demo Configuration");
    _configuration_adapter->add_page();
    _configuration_adapter->add_page();
    emit configuration_loaded();
}
ConfigurationAdapter* ManagerAdapter::configuration_adapter() {
    return _configuration_adapter;
}