#pragma once
#include <string>
#include "../base_style.h"

class ScatterStyle: public BaseStyle {
public:
    explicit ScatterStyle();
    virtual ~ScatterStyle() = default;
    std::string line_style;
    std::string line_color;
    std::string marker;
    int marker_size         = 5;
    virtual std::unique_ptr<BaseStyle> clone() const override;
    QString type_name() const override { return "Scatter"; }
};