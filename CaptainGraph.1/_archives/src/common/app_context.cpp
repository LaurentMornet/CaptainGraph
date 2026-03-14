#include "app_context.h"
AppContext::AppContext(QWidget * _parent) { if(_parent) {} /* pour éviter le warning "unused" */ }
void AppContext::check() {
    std::cout << "Check!" << std::endl;
}