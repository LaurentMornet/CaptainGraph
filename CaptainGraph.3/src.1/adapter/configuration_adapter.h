#pragma once
#include <QObject>
#include "../common/configuration.h"
#include "../adapter/layout_adapter.h"

class ConfigurationAdapter: public QObject {
    Q_OBJECT
private:
    Configuration* _configuration;
    QList<LayoutAdapter*> _layout_adapters;
public:
    explicit ConfigurationAdapter(Configuration* configuration, QObject* parent=nullptr);
    virtual ~ConfigurationAdapter() = default;
    unsigned short layouts_count() const;
    void set_label(const QString& label);
    QString layout_title(unsigned short idx) const;
    void set_layout_title(const unsigned short idx, const QString& new_title);
    LayoutAdapter* add_layout();
    LayoutAdapter* layout_adapter(unsigned short idx);
};