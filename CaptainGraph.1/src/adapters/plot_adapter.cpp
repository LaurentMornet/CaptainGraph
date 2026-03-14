#include "plot_adapter.h"

PlotAdapter::PlotAdapter(Plot* plot, QObject* parent): QObject(parent), _plot(plot) {}

Plot* PlotAdapter::plot() const {
    return _plot;
}

QString PlotAdapter::label() const {
    return QString::fromStdString(_plot->label());
}

void PlotAdapter::set_label(const QString& lbl) {
    if(lbl.toStdString() == _plot->label()) return;
    _plot->set_label(lbl.toStdString());
    emit label_changed(lbl);
}

QString PlotAdapter::feature_value(int i) const {
    return QString::fromStdString(_plot->feature_value(i));
}

void PlotAdapter::set_feature_value(int i, const QString& value) {
    if(value.toStdString() == _plot->feature_value(i)) return;
    _plot->set_feature_value(i, value.toStdString());
    emit feature_changed(i, value);
}

int PlotAdapter::features_count() {
    return static_cast<int>(_plot->features_count());
}