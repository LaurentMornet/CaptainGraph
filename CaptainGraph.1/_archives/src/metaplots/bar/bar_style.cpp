#include "bar_style.h"
BarStyle::BarStyle() : fill_color("red") {}
std::unique_ptr<BaseStyle> BarStyle::clone() const {
    return std::make_unique<BarStyle>(*this);
}