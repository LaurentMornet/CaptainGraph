#include "page_adapter.h"
#include "../common/page.h"

PageAdapter::PageAdapter(Page* page, QObject* parent):
    QObject(parent), _page(page) {
}
void PageAdapter::add_rectangle(const Rectangle& rect) {
    _page->add_rectangle(rect);
}
void PageAdapter::add_rectangle(double left, double top, double width, double height) {
    Rectangle rect(left, top, width, height);
    _page->add_rectangle(rect);
}
void PageAdapter::remove_rectangle(const short &irect) {
    _page->remove_rectangle(irect);
}
short PageAdapter::rectangles_count() const {
    return _page->rectangles_count();
}
Rectangle* PageAdapter::rectangle(const short& irect) {
    return _page->rectangle(irect);
}
void PageAdapter::set_rectangle_graph(const short& irect, const QString& igraph) {
    _page->set_rectangle_graph(irect, igraph.toStdString());
}
void PageAdapter::unset_rectangle_graph(const short& irect) {
    _page->unset_rectangle_graph(irect);
}
QString PageAdapter::label() const {
    return QString::fromStdString(_page->label());
}