#pragma once

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

#include "../../common/singleton.h"

class PageConfigFrame: public QFrame, public Singleton<PageConfigFrame> {
    Q_OBJECT
    friend class Singleton;
private:
    void _setup();
    explicit PageConfigFrame(QWidget * parent = nullptr);
    virtual ~PageConfigFrame() override = default;
};