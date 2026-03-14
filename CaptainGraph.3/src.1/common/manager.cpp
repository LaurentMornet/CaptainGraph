#include "manager.h"
#include <iostream>

Manager::Manager() {
}

Configuration& Manager::configuration() { 
    return _configuration; 
}