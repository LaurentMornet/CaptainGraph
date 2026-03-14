#pragma once

#include <QString>
#include <QObject>
#include "../common/plot.h"

class PlotAdapter: public QObject {
    Q_OBJECT
private:
    Plot * _plot;
public:
    PlotAdapter(Plot* plot, QObject* parent=nullptr);
    Plot* plot() const;
    QString label() const;
    void set_label(const QString& lbl);
    QString feature_value(int i) const;
    void set_feature_value(int i, const QString& value);
    int features_count();
signals:
    void label_changed(const QString&);
    void feature_changed(int, const QString&);
};