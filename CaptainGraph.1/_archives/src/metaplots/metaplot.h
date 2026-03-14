#pragma once

#include <string>
#include <map>

#include "../common/metagraph.h"
#include "base_style.h"

class MetaPlot {
private:
    static inline int __global__id = 0;
    MetaGraph * _ancestor;
    std::string _title;
    std::map<std::string, std::string> _mandatory_data;
    std::map<std::string, std::string> _optional_data;
protected:
    std::shared_ptr<BaseStyle> _style;
public:
    explicit MetaPlot(std::string prefix="");
    virtual ~MetaPlot() = default;
    
    std::shared_ptr<BaseStyle> & style();
    void set_style(std::unique_ptr<BaseStyle> & style);

    void set_ancestor(MetaGraph * ancestor);
    MetaGraph * ancestor();

    void set_title(std::string title);
    std::string & title();
    
    virtual std::string type_name() const = 0;
    virtual std::string to_plotly() = 0;
    virtual std::string to_mpl() = 0;

    void set_mandatory_data(std::string id, std::string content);
    void apply_style(BaseStyle * base_style);
};