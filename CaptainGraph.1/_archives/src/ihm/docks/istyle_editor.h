#pragma once
#include <memory>
#include <QtPlugin>
#include "../../common/base_style.h"

// #include <QObject>

class IStyleEditor {
public:
    virtual ~IStyleEditor() = default;
    virtual void set_style(const BaseStyle * style) = 0;
    virtual std::unique_ptr<BaseStyle> get_style() const = 0;
};

#define IStyleEditor_iid "com.myapp.IStyleEditor"
Q_DECLARE_INTERFACE(IStyleEditor, IStyleEditor_iid)
