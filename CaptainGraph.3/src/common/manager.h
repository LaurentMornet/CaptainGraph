#pragma once

#include <memory>
#include <mutex>

#include "configuration.h"

class Manager {
private:
    Configuration _configuration;
public:
    explicit Manager();
    virtual ~Manager()=default;
    Manager(const Manager&)=delete;
    Manager& operator=(const Manager&)=delete;
    Configuration& configuration();
};