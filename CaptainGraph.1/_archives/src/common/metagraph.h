#pragma once

#include <string>
#include <vector>

#include <QDebug>
#include <QString>
#include <QIODevice>
#include <QFile>
#include <QTextStream>

// #include "../metaplots/metaplot.h"
class MetaPlot;

class MetaGraph {
private:
    static inline int __global__id = 0;
    std::string _title;
    std::string _projection;
    std::vector<MetaPlot*> _v_metaplots;
public:
    explicit MetaGraph();
    virtual ~MetaGraph() = default;
    std::string & title();
    void set_title(std::string & title);
    void add_metaplot(MetaPlot * metaplot);
    MetaPlot * metaplot(int idx);
    int metaplots_count();
    std::string html_base_2d();
};