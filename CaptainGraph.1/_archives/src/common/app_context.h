#pragma once

#include <iostream>
#include <memory>

#include <QObject>

#include "singleton.h"

class AppContext: public QObject, public Singleton<AppContext> {
    Q_OBJECT
    friend class Singleton<AppContext>;
private:
    // singleton
    explicit AppContext(QWidget * parent = nullptr);
public:
    virtual ~AppContext() = default;
    void check();
};