#pragma once
#include <string>
#include "../base_style.h"

class BarStyle: public BaseStyle {
public:
    explicit BarStyle();
    virtual ~BarStyle()         = default;
    std::string fill_color;
    double      border_width    = 1.0;
    virtual std::unique_ptr<BaseStyle> clone() const override;
    QString type_name() const override { return "Bar"; }
};