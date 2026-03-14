#pragma once

#include "../common/manager.h"
#include "../adapter/configuration_adapter.h"

class ManagerAdapter: public QObject {
    Q_OBJECT
private:
    Manager* _manager = nullptr;
    ConfigurationAdapter* _configuration_adapter = nullptr;
protected:
public:
    explicit ManagerAdapter(Manager* manager);
    virtual ~ManagerAdapter() = default;
    ConfigurationAdapter* configuration_adapter();
    void load_configuration();
signals:
    void configuration_loaded();
};