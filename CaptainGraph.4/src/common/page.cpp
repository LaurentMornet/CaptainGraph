#include "page.h"
#include "configuration.h"

#include <sstream>

/*static*/unsigned int Page::__s__id__ = 0;

Page::Page(Configuration* configuration):
    _configuration(configuration) {
    // label auto
    std::ostringstream sbuffer;
    sbuffer << "Page " << ++Page::__s__id__;
    set_label(sbuffer.str());
    // rectangle initial
    add_rectangle(Rectangle(0.1, 0.1, 0.5, 0.5));
}
Configuration* Page::configuration() const {
    return _configuration;
}
std::string Page::label() const {
    return _label;
}
void Page::set_label(const std::string& label) {
    _label = label;
}
void Page::add_rectangle(const Rectangle& rect) {
    _rectangles.push_back(std::make_unique<Rectangle>(rect.left, rect.top, rect.width, rect.height));
}
void Page::remove_rectangle(const unsigned short& irect) {
    if(irect >= rectangles_count()) return;
    _rectangles.erase(_rectangles.begin() + irect);
    _graph_map.erase(irect);
}
unsigned short Page::rectangles_count() const {
    return static_cast<unsigned short>(_rectangles.size());
}
Rectangle* Page::rectangle(const unsigned short& irect) {
    if(irect >= rectangles_count()) return nullptr;
    return _rectangles[irect].get();
}
void Page::set_rectangle_graph(const unsigned short& irect, const std::string& igraph) {
    if(irect >= rectangles_count()) return;
    _graph_map[irect] = igraph;
}
void Page::unset_rectangle_graph(const unsigned short& irect) {
    if(irect >= rectangles_count()) return;
    _graph_map.erase(irect);
}