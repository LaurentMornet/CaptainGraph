#pragma once

#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFormLayout>

#include "../../common/metagraph.h"
#include "../../common/singleton.h"

class GraphConfigFrame: public QFrame, public Singleton<GraphConfigFrame> {
    Q_OBJECT
    friend class Singleton;
signals:
    void selected(MetaGraph * metagraph);
private:
    MetaGraph * _managed_metagraph;
    QLineEdit * _edt_title;
    void _setup();
    explicit GraphConfigFrame(QWidget * parent = nullptr);
    virtual ~GraphConfigFrame() override = default;
    void _set_metagraph(MetaGraph * metagraph);
private slots:
    void slot_selected(MetaGraph * metagraph);
public:
    MetaGraph * metagraph();
};