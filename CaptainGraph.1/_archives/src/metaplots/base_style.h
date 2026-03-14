#pragma once
#include <memory>
#include <QString>
class BaseStyle {
public:
    explicit BaseStyle() = default;
    virtual ~BaseStyle() = default;
    // shared properties
    bool    show_legend     = true;
    double  opacity         = 1.0;
    virtual std::unique_ptr<BaseStyle> clone() const = 0;
    virtual QString type_name() const = 0;
};