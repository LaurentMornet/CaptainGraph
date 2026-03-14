#pragma once

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebChannel>
#include <QTimer>
#include <QVector>
#include <QString>
#include <QMouseEvent>

#include "../common/metagraph.h"
#include "../ihm/docks/graph_config_frame.h"
#include "../ihm/docks/plot_config_frame.h"
#include "../metaplots/metaplot.h"
#include "../metaplots/scatter/scatter_metaplot.h"

class Bridge: public QObject {
Q_OBJECT
public slots:
    void mouse_pressed();
public:
    explicit Bridge(QWidget * parent=nullptr);
    virtual ~Bridge() = default;
};

class GraphFrame: public QFrame {
Q_OBJECT
private:
    Bridge * _bridge;
    MetaGraph _metagraph;
    QWebEngineView * _view;
    QString _to_js_array(const QVector<double>& data);
public:
    explicit GraphFrame(QWidget * parent = nullptr);
    virtual ~GraphFrame() = default;
    MetaGraph * metagraph();
    void update_graph();
};