#pragma once
#include <QColor>
#include <QObject>
#include <QVariant>
#include <QIcon>
#include "../common/plot.h"
class PlotAdapter: public QObject {
    Q_OBJECT
private:
    Plot* _plot;
public:
    explicit PlotAdapter(Plot* plot, QObject* parent = nullptr);
    Plot* plot() const;
    // interface simple
    PlotType type() const;
    int type_as_int() const;
    QString label() const;
    void set_type(const PlotType& type);
    void set_type(const unsigned int& idx);
    void set_label(const QString& label);
    static unsigned int plot_type_count();
    static QString plot_type_label(unsigned int idx);
    // gestion des features
    int features_count() const;
    QString feature(unsigned int idx) const;
    QString feature_label(unsigned int idx) const;
    void set_feature_value(unsigned int idx, const QString& value);
    // gestion du style
    QList<QString> style_keys();
    QVariant styledef(const QString& style_item_id);
    void set_styledef(const QString& style_item_id, const QVariant& value);
    void reset_styledef();
    static QMap<PlotType, QString> plots_types_map();
signals:
    void type_changed(int id, PlotType type);
    void label_changed(QString);
    void feature_value_changed(unsigned int, QString);
    void styledef_changed(QString, QVariant);
};