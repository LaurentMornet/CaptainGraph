#pragma once

#include <string>
#include <vector>
#include "plot.h"

class Graph {
private:
    static short _s_count_;
    short _id;
    std::string _title;
    std::string _x_label="";
    std::string _y_label="";
    std::string _z_label="";
    bool        _grid=true;
    bool        _axis_equal=false;
    std::vector<Plot> _plots;
protected:
public:
    explicit Graph();
    virtual ~Graph()=default;
    // accesseurs / modificateurs
    short id() const;
    std::string title() const;
    void set_title(const std::string title);
    std::string x_label() const;
    void set_x_label(const std::string& label);
    std::string y_label() const;
    void set_y_label(const std::string& label);
    std::string z_label() const;
    void set_z_label(const std::string& label);
    bool grid() const;
    void set_grid(bool);
    bool axis_equal() const;
    void set_axis_equal(bool);
    // gestion des plots
    Plot* add_plot();
    unsigned int plots_count();
    Plot& plot(unsigned int idx);
    Plot& last_plot();
    void swap_plots(int i0, int i1);
    void delete_plot(int i);
};