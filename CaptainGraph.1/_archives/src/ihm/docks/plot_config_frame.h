#pragma once

#include <QFrame>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QIcon>
#include <QRadioButton>
#include <QStyleFactory>
#include <QStackedWidget>

#include "../../common/singleton.h"
#include "../forms_widgets/auto_label_widget.h"
#include "../forms_widgets/linestyle_widget.h"
#include "../../metaplots/metaplot.h"
#include "../../metaplots/scatter/scatter_style_frame.h"
#include "../../metaplots/scatter/scatter_metaplot.h"
#include "../../metaplots/bar/bar_style_frame.h"
#include "../../metaplots/generic_style_frame.h"
#include "../../metaplots/istyle_editor.h"
#include "../../metaplots/style_field_desc.h"
#include "../../metaplots/style_mapper.h"

class PlotConfigFrame: public QFrame, public Singleton<PlotConfigFrame> {
    Q_OBJECT
    friend class Singleton;
signals:
    void selected(MetaPlot * metaplot);
private:
    bool _basic;
    QVBoxLayout * _layout;
    MetaPlot * _metaplot;
    AutoLabelWidget * _edt_title;
    QComboBox * _cb_type;

    QStackedWidget * _stack_style;
    GenericStyleFrame * _scatter_frame;
    BarStyleFrame * _bar_frame;
    std::unique_ptr<MetaPlot> _current_plot;

    void _setup();
    explicit PlotConfigFrame(QWidget * parent = nullptr);
    virtual ~PlotConfigFrame() override = default;
private slots:
    void _cb_auto_title();
    void _cb_type_changed(int selection);
    void _cb_validate();
    void slot_selected(MetaPlot * metaplot);
};