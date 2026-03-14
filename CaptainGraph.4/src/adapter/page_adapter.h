#pragma once

#include <QObject>

class Rectangle;
class Page;

class PageAdapter: public QObject {
    Q_OBJECT
private:
    Page* _page;
public:
    QString label() const;
    explicit PageAdapter(Page* page, QObject* parent);
    void add_rectangle(const Rectangle& rect);
    void add_rectangle(double left, double top, double width, double height);
    void remove_rectangle(const short &irect);
    short rectangles_count() const;
    Rectangle* rectangle(const short& irect);
    void set_rectangle_graph(const short& irect, const QString& igraph);
    void unset_rectangle_graph(const short& irect);
};