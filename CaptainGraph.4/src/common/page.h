#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

class Configuration;

class Rectangle {
public:
    double left     = 0.0;
    double top      = 0.0;
    double width    = 0.0;
    double height   = 0.0;
    Rectangle(double l, double t, double w, double h) {
        left=l; top=t; width=w; height=h;
    }
};

class Page {
    static  unsigned int                    __s__id__;
private:
    Configuration*                          _configuration;
    std::string                             _label;
    std::vector<std::unique_ptr<Rectangle>> _rectangles;
    std::map<unsigned short, std::string>   _graph_map;
public:
    explicit Page(Configuration* configuration);
    Configuration* configuration() const;
    std::string label() const;
    void set_label(const std::string& label);
    void add_rectangle(const Rectangle& rect);
    void remove_rectangle(const unsigned short& irect);
    unsigned short rectangles_count() const;
    Rectangle* rectangle(const unsigned short& irect);
    void set_rectangle_graph(const unsigned short& irect, const std::string& igraph);
    void unset_rectangle_graph(const unsigned short& irect);
};