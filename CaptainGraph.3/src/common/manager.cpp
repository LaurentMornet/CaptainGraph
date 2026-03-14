#include "manager.h"

Manager::Manager() {
    _configuration.set_label("Demo Configuration with Smart Pointers");
    _configuration.add_layout();
    _configuration.set_layout_title(0, "First layout");
    _configuration.sublayout(0, "A").split(LayoutType::Horizontal);
    _configuration.sublayout(0, "B").split(LayoutType::Vertical);
}

Configuration& Manager::configuration() { 
    return _configuration; 
}